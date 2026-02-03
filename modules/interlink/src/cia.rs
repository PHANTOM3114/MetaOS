use dirs::data_dir;
use matrix_sdk::{
    Client, authentication::matrix::MatrixSession, config::SyncSettings, ruma::{RoomId, events::room::message::RoomMessageEventContent}
};
use std::path::PathBuf;
use anyhow::{anyhow, Context, Result}; 
use log::{info, warn, error};
use crate::common::{common::CriticalLvl, config::CiaBotFullConfig};

fn get_db_path() -> Result<PathBuf> {
    let mut db_path = data_dir().context("Failed to get data directory")?;
    db_path.push("ar-s");
    std::fs::create_dir_all(&db_path)?;
    db_path.push("matrix-store");
    Ok(db_path)
}

pub async fn build_and_login(config: &CiaBotFullConfig) -> anyhow::Result<Client> {
    let db_path = get_db_path()?;
    let db_passphrase = crate::auth::get_or_create_db_passphrase(&config.username)?;

    info!("Initializing Matrix Client...");

    let client_build_result = Client::builder()
        .homeserver_url(&config.homeserver_url)
        .sqlite_store(&db_path, Some(&db_passphrase))
        .build()
        .await;

    let client = match client_build_result {
        Ok(c) => c,
        Err(e) => {
            warn!("Failed to open database (Crypto Error?): {}. Wiping and retrying...", e);
            
            if db_path.exists() {
                std::fs::remove_dir_all(&db_path).context("Failed to delete corrupted DB")?;
            }
            
            Client::builder()
                .homeserver_url(&config.homeserver_url)
                .sqlite_store(&db_path, Some(&db_passphrase))
                .build()
                .await
                .context("Failed to build client after DB reset")?
        }
    };
    
    let session_restored = if let Ok(json_token) = crate::auth::load_session_json(&config.username) {
        info!("Found session token in KWallet.");
        match serde_json::from_str::<MatrixSession>(&json_token) {
            Ok(session) => {
                if let Err(e) = client.restore_session(session).await {
                    warn!("Failed to restore session: {}. Token expired?", e);
                    false
                } else {
                    info!("Session restored successfully!");
                    true
                }
            },
            Err(e) => {
                error!("Corrupted session JSON: {}", e);
                false
            }
        }
    } else {
        info!("No session token in KWallet. Fresh login required.");
        false
    };

    
    if !session_restored {
        match perform_login_action(&client, config).await {
            Ok(_) => {},
            Err(e) => {
                error!("Login failed with existing DB: {}. Nuking everything.", e);
                drop(client);
                std::fs::remove_dir_all(&db_path).ok();
                
                return Box::pin(establish_fresh_client(config)).await;
            }
        }
    }

    let _ = client.sync_once(SyncSettings::default()).await;

    Ok(client)
}

async fn perform_login_action(client: &Client, config: &CiaBotFullConfig) -> Result<()> {
    info!("Logging in via password...");
    
    client.matrix_auth()
        .login_username(&config.username, &config.password)
        .initial_device_display_name("AR-S Interlink Daemon")
        .await
        .context("Failed to login")?;

    if let Some(session) = client.matrix_auth().session() {
        let json = serde_json::to_string(&session)?;
        crate::auth::save_session_json(&config.username, &json)?;
        info!("Session token saved to KWallet securely.");
    }
    
    let _ = client.sync_once(SyncSettings::default()).await;
    
    Ok(())
}

async fn establish_fresh_client(config: &CiaBotFullConfig) -> Result<Client> {
    let db_path = get_db_path()?;
    let db_passphrase = crate::auth::get_or_create_db_passphrase(&config.username)?;

    let client = Client::builder()
        .homeserver_url(&config.homeserver_url)
        .sqlite_store(&db_path, Some(&db_passphrase))
        .build()
        .await?;

    perform_login_action(&client, config).await?;
    Ok(client)
}

pub async fn send_message_to_room(
    cia_bot: &matrix_sdk::Client,
    room_id: &RoomId, 
    message_to_send: &str, 
    lvl: CriticalLvl, 
    module_name: &str
) -> anyhow::Result<()> {
    
    let formatted_message = format!("[{}] {} ({})", lvl.as_str(), message_to_send, module_name);
    let joined_room = cia_bot.joined_rooms()
        .into_iter()
        .find(|r| r.room_id() == room_id)
        .ok_or_else(|| anyhow!("Bot is not found in 'joined' list for target room: {}", room_id))?;

    joined_room.send(RoomMessageEventContent::text_plain(formatted_message))
        .await
        .context("Failed to send message")?;

    info!("Message sent successfully");

    Ok(())
}