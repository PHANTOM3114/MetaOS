use std::env;
use matrix_sdk::{
    Client,
    ruma::{
        events::room::message::RoomMessageEventContent,
        RoomId,
        OwnedRoomId
    },
};
use tracing::{info}; 
use anyhow::{anyhow, Context}; 

use crate::common::common::CriticalLvl;

pub struct ClientData {
    client_homeserver_url: String,
    client_username: String,
    client_password: String, 
    client_target_room_id: String
}

pub async fn build_and_login() -> anyhow::Result<(Client, OwnedRoomId)> {
    println!("ENV scanning...");
    let homeserver_url = env::var("MATRIX_HOMESERVER")?;
    let username = env::var("MATRIX_BOT_USERNAME")?;
    let password = env::var("MATRIX_BOT_PASSWORD")?;
    let room_id_str = env::var("MATRIX_TARGET_ROOM_ID")?;

    println!("client_data filling...");

    let target_room_id = RoomId::parse(&room_id_str)?;

    let client_data = ClientData {
        client_homeserver_url: homeserver_url,
        client_username: username,
        client_password: password,
        client_target_room_id: room_id_str,
    };
    
    let client = Client::builder()
        .homeserver_url(client_data.client_homeserver_url)
        .build()
        .await?;
    
    client.matrix_auth()
        .login_username(&client_data.client_username, &client_data.client_password)
        .initial_device_display_name("CIA_BOT")
        .await?;

    println!("returning Ok()");
    Ok((client, target_room_id))
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