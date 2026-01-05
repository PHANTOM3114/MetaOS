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

async fn login_and_send_to_room(
    homeserver_url: String,
    username: String,
    password: String,
    target_room_id_str: String,
) -> anyhow::Result<()> {
    info!("Attempting to create client for {}", homeserver_url);
    let client = Client::builder().homeserver_url(homeserver_url).build().await?;
    info!("Client created. Attempting login as {}", username);

    match client
        .matrix_auth()
        .login_username(&username, &password)
        .initial_device_display_name("rust-hello-bot-simple")
        .await
    {
        Ok(_) => {
            let user_id = client.user_id().ok_or_else(|| anyhow!("Failed to get User ID after login"))?;
            info!("Login successful! User ID: {}", user_id);
        }
        Err(e) => {
            error!("Login error: {:?}", e);
            return Err(e.into());
        }
    }

    let room_id = RoomId::parse(target_room_id_str)?;
    info!("Attempting to get room with ID: {}", room_id);

    if let Some(room) = client.get_room(&room_id) {
        if room.state() == RoomState::Joined {
            info!("Room {} found and we're in it. Sending message...", room_id);
            let content = RoomMessageEventContent::text_plain("Hello! Bot successfully started (sending to specific room).");
            match room.send(content).await {
                Ok(response) => info!("Message sent successfully. Event ID: {}", response.event_id),
                Err(e) => error!("Error sending message: {:?}", e),
            }
        } else {
            error!("Found room {}, but bot is not a member (state: {:?}).", room_id, room.state());
            return Err(anyhow!("Bot is not a member of the target room"));
        }
    } else {
        error!("Room with ID {} not found in client's local state.", room_id);
        info!("Make sure the room ID is correct and the bot has received information about it.");
        return Err(anyhow!("Target room not found"));
    }

    info!("Operation completed.");
    Ok(())
}

#[tokio::main]
async fn main() -> anyhow::Result<()> {
    use dotenv::dotenv;
    dotenv().ok();

    tracing_subscriber::fmt()
        .with_max_level(tracing::Level::INFO)
        .init();

    let homeserver_url = env::var("MATRIX_HOMESERVER")?;
    let username = env::var("MATRIX_BOT_USERNAME")?;
    let password = env::var("MATRIX_BOT_PASSWORD")?;
    let target_room_id = env::var("MATRIX_TARGET_ROOM_ID")?;

    info!("Starting login and sending to room {}...", target_room_id);

    info!("Attempting to create client for {}", homeserver_url);
    let client = Client::builder().homeserver_url(homeserver_url).build().await?;
    info!("Client created. Attempting login as {}", username);

    match client
        .matrix_auth()
        .login_username(&username, &password)
        .initial_device_display_name("rust-hello-bot-simple")
        .await
    {
        Ok(_) => {
            let user_id = client.user_id().ok_or_else(|| anyhow!("Failed to get User ID after login"))?;
            info!("Login successful! User ID: {}", user_id);
        }
        Err(e) => {
            error!("Login error: {:?}", e);
            return Err(e.into());
        }
    }

    info!("Performing initial synchronization (sync_once)...");
    match client.sync_once(Default::default()).await {
        Ok(_) => info!("Initial synchronization successful."),
        Err(e) => {
            error!("Initial synchronization error: {:?}", e);
            return Err(e.into());
        }
    }

    async fn send_to_room(client: Client, target_room_id_str: String) -> anyhow::Result<()> {
        let room_id = RoomId::parse(&target_room_id_str)?;
        info!("Attempting to get room with ID: {}", room_id);

        if let Some(room) = client.get_room(&room_id) {
            if room.state() == RoomState::Joined {
                info!("Room {} found and we're in it. Sending message...", room_id);
                let content = RoomMessageEventContent::text_plain("Hello! Bot successfully started (sending to specific room).");
                match room.send(content).await {
                    Ok(response) => info!("Message sent successfully. Event ID: {}", response.event_id),
                    Err(e) => error!("Error sending message: {:?}", e),
                }
            } else {
                error!("Found room {}, but bot is not a member (state: {:?}).", room_id, room.state());
                return Err(anyhow!("Bot is not a member of the target room"));
            }
        } else {
            error!("Room with ID {} not found in client's local state after sync_once.", room_id);
            info!("Make sure the room ID is correct and the bot is a member.");
            return Err(anyhow!("Target room not found"));
        }
        Ok(())
    }

    if let Err(e) = send_to_room(client, target_room_id).await {
         error!("Program terminated with error during sending: {:?}", e);
         exit(1);
    }

    info!("Program completed successfully.");
    Ok(())
}