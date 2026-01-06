use std::{env, process::exit};
use matrix_sdk::{
    RoomState,
    room::Room,
    ruma::{
        events::room::message::RoomMessageEventContent,
        RoomId,
    },
    Client,
};
use tracing::{info, error};
use anyhow::anyhow;

async fn send_hello_to_room(client: &Client, room_id_str: &str) -> anyhow::Result<()> {
    let room_id = RoomId::parse(room_id_str)?;
    info!("Looking for room: {}", room_id);

    if let Some(room) = client.get_room(&room_id) {
        if room.state() == RoomState::Joined {
            info!("Room found. Sending message...");
            let content = RoomMessageEventContent::text_plain("Hello! Refactored bot is here.");
            
            match room.send(content).await {
                Ok(response) => info!("Success! Event ID: {}", response.event_id),
                Err(e) => error!("Failed to send: {:?}", e),
            }
        } else {
            return Err(anyhow!("Bot is not fully joined to this room"));
        }
    } else {
        return Err(anyhow!("Room not found in local state (did you sync?)"));
    }
    Ok(())
}
#[tokio::main]
async fn main() -> anyhow::Result<()> {
    tracing_subscriber::fmt()
        .with_max_level(tracing::Level::INFO)
        .init();

    let exe_path = std::env::current_exe()?;
    let exe_dir = exe_path.parent().ok_or(anyhow!("Failed to get binary directory"))?;
    let env_path = exe_dir.join(".env");

    info!("Looking for .env file at: {:?}", env_path);

    if let Err(e) = dotenv::from_path(&env_path) {
        info!("Warning: .env file not found or not readable at {:?}. Error: {}", env_path, e);
    } else {
        info!("Successfully loaded .env file");
    }

    let homeserver_url = env::var("MATRIX_HOMESERVER")?;
    let username = env::var("MATRIX_BOT_USERNAME")?;
    let password = env::var("MATRIX_BOT_PASSWORD")?;
    let target_room_id = env::var("MATRIX_TARGET_ROOM_ID")?;

    info!("Connecting to {}", homeserver_url);
    let client = Client::builder().homeserver_url(homeserver_url).build().await?;

    client.matrix_auth()
        .login_username(&username, &password)
        .initial_device_display_name("rust-bot-clean")
        .await?;
    info!("Logged in as {}", client.user_id().unwrap());

    info!("Syncing...");
    client.sync_once(Default::default()).await?;
    info!("Synced!");

    if let Err(e) = send_hello_to_room(&client, &target_room_id).await {
        error!("Fatal error: {:?}", e);
        exit(1);
    }

    info!("Done.");
    Ok(())
}