use keyring::Entry;
use anyhow::Result;
use rand::{distr::Alphanumeric, Rng};

const SERVICE_ACCOUNT: &str = "ars-interlink";
const SERVICE_DB: &str = "ars-interlink-db-key";
const SERVICE_TOKEN: &str = "ars-interlink-session-token";

pub fn get_secret_key(username: &str) -> Result<String> {
    Entry::new(SERVICE_ACCOUNT, username)?.get_password().map_err(|e| e.into())
}

pub fn set_secret_in_keyring(username: &str, password: &str) -> Result<()> {
    Entry::new(SERVICE_ACCOUNT, username)?.set_password(password).map_err(|e| e.into())
}

pub fn get_or_create_db_passphrase(username: &str) -> Result<String> {
    let entry = Entry::new(SERVICE_DB, username)?;
    match entry.get_password() {
        Ok(pass) => Ok(pass),
        Err(_) => {
            let new_key: String = rand::rng()
                .sample_iter(&Alphanumeric)
                .take(32)
                .map(char::from)
                .collect();
            entry.set_password(&new_key)?;
            Ok(new_key)
        }
    }
}

pub fn save_session_json(username: &str, json: &str) -> Result<()> {
    let entry = Entry::new(SERVICE_TOKEN, username)?;
    entry.set_password(json).map_err(|e| e.into())
}

pub fn load_session_json(username: &str) -> Result<String> {
    let entry = Entry::new(SERVICE_TOKEN, username)?;
    entry.get_password().map_err(|e| e.into())
}