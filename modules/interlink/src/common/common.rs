use matrix_sdk::{
    Client,
    ruma::OwnedRoomId,
};
use async_trait::async_trait;

use crate::cia::send_message_to_room;

#[derive(Debug, Clone, Copy)]
pub enum CriticalLvl {
    Info,
    Critical
}

impl CriticalLvl {
    pub fn as_str(&self) -> &'static str {
        match self {
            Self::Info => "INFO",
            Self::Critical => "CRITICAL_FAILURE",
        }
    }
}

#[derive(Clone)]
pub struct CiaBotData {
    client: Client,
    room_id: OwnedRoomId
}

impl CiaBotData {
    pub fn new(client: Client, room_id: OwnedRoomId) -> Self {
        Self { client, room_id }
    }
}

#[async_trait]
pub trait ModuleMessageAccept: Send + Sync {
    async fn on_crash_error(&self, module_name: &str, message_to_send: &str);
    async fn on_info(&self, module_name: &str, message_to_send: &str);
}

#[async_trait]
impl ModuleMessageAccept for CiaBotData {
    async fn on_crash_error(&self, module_name: &str, message_to_send: &str) {
        if let Err(e) = send_message_to_room(
            &self.client, 
            &self.room_id, 
            message_to_send, 
            CriticalLvl::Critical, 
            module_name
        ).await {
            tracing::error!("Failed to process crash error: {}", e);
        }
    }

    async fn on_info(&self, module_name: &str, message_to_send: &str) {
        if let Err(e) = send_message_to_room(
            &self.client, 
            &self.room_id, 
            message_to_send, 
            CriticalLvl::Info, 
            module_name
        ).await {
            tracing::error!("Failed to process info message: {}", e);
        }
    }
}