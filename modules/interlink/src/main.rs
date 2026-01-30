mod cia;
mod common;

mod listeners {
    pub mod sonar;
}

use crate::listeners::sonar::SonarMessageAcceptProxy;
use crate::common::common::{CiaBotData, ModuleMessageAccept};

use anyhow::{anyhow};
use tracing::{info, error};
use zbus::{Connection};
use futures_util::stream::StreamExt;

#[tokio::main]
async fn main() -> anyhow::Result<()> {
    tracing_subscriber::fmt::init();

    let exe_path = std::env::current_exe()?;
    let exe_dir = exe_path.parent().ok_or(anyhow!("Failed to get binary directory"))?;
    let env_path = exe_dir.join(".env");
    
    if let Err(e) = dotenv::from_path(&env_path) {
        info!("Warning: .env file not found: {}", e);
    } else {
        info!("Successfully loaded .env file");
    }

    let (client, target_room_id) = cia::build_and_login().await?;
    info!("Logged in as {}", client.user_id().unwrap());

    info!("Syncing...");
    client.sync_once(Default::default()).await?;
    info!("Synced!");

    let bot_agent = CiaBotData::new(client, target_room_id);
    
    bot_agent.on_info("Interlink", "System Online. Secure Uplink Established.").await;

    let connection = Connection::session().await?;
    let proxy = SonarMessageAcceptProxy::new(&connection).await?;

    let mut stop_stream = proxy.receive_sonar_work_stop().await?;
    let mut crash_stream = proxy.receive_sonar_crash().await?;

    info!("Listening for signals...");

    loop {
        tokio::select! {
            Some(signal) = stop_stream.next() => {
                let args = signal.args().expect("Failed to parse args");
                let msg: &str = args.exit_message;
                
                info!("Received Stop Signal: {}", msg);
                
                bot_agent.on_info("Sonar", &format!("Graceful Shutdown: {}", msg)).await;
            }
            
            Some(signal) = crash_stream.next() => {
                let args = signal.args().expect("Failed to parse args");
                let msg: &str = args.crash_message;

                error!("Received CRASH: {}", msg);

                bot_agent.on_crash_error("Sonar", &format!("System CRASH: {}", msg)).await;
            }
        }
    }
}