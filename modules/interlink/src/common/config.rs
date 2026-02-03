use serde::Deserialize;
use std::fs;
use std::result::Result::{Ok, Err};
use std::io::{self, Write};
use anyhow::{Context, Result};
use dirs::config_dir;

#[derive(Deserialize)]
struct ConfigFile {
    matrix: MatrixConfig,
}

#[derive(Deserialize)]
struct MatrixConfig {
    homeserver_url: String,
    username: String,
    target_room_id: String,
    owner_id: String, 
}

pub struct CiaBotFullConfig {
    pub homeserver_url: String,
    pub username: String,
    pub ownername: String,
    pub target_room_id: String,
    pub password: String
}

pub fn load_config() -> Result<CiaBotFullConfig> {
    let mut config_path = config_dir().context("Could not find config directory")?;

    config_path.push("ars");
    config_path.push("interlink.toml");

    let content = fs::read_to_string(&config_path).context(format!("Failed to read config file at {:?}", config_path))?;
    let config_file: ConfigFile = toml::from_str(&content).context("Failed to parse interlink.toml")?;

    let username = &config_file.matrix.username;

    let password = match crate::auth::get_secret_key(&config_file.matrix.username) {
        Ok(pass) => {
            println!("Password has found");
            pass
        }

        Err(_) => {
            println!("Password not found in keyring for user: {}", username);
            print!("Please enter Matrix password: ");
            io::stdout().flush()?; 

            let mut input = String::new();
            io::stdin().read_line(&mut input)?;
            
            let trimmed_pass = input.trim().to_string(); 

            crate::auth::set_secret_in_keyring(username, &trimmed_pass)?;
            println!("Password saved to keyring via Rust!");

            trimmed_pass
        }
    };

    Ok(CiaBotFullConfig { 
        homeserver_url: config_file.matrix.homeserver_url,
        username: config_file.matrix.username,
        ownername: config_file.matrix.owner_id,
        target_room_id: config_file.matrix.target_room_id,
        password,
    })
}