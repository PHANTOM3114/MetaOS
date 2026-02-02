use matrix_sdk::{
    Client,
    ruma::{
        events::room::message::RoomMessageEventContent,
        RoomId,
    },
};
use tracing::{info}; 
use anyhow::{anyhow, Context}; 

use crate::common::{common::CriticalLvl, config::CiaBotFullConfig};

pub async fn build_and_login(config: &CiaBotFullConfig) -> anyhow::Result<Client> {
    let client = Client::builder()
        .homeserver_url(&config.homeserver_url)
        .build()
        .await?;
    
    client.matrix_auth()
        .login_username(&config.username, &config.password)
        .initial_device_display_name("C.I.A")
        .await?;

    println!("returning Ok()");
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