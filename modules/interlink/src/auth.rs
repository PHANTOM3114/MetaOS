use keyring::Entry;
use anyhow::{Result, Context};

pub fn get_secret_key(username: &str) -> Result<String> {
    println!("Asking KWallet for service: 'ars-interlink', username: '{}'", username);

    let entry = Entry::new("ars-interlink", username).context("Failed to create keyring entry")?;
    let password = entry.get_password().context("Failed to get password from KWallet. Have you saved it via Deck?")?;

    Ok(password)
}

pub fn set_secret_in_keyring(username: &str, password: &str) -> Result<()> {
    let entry = Entry::new("ars-interlink", username)
        .context("Failed to create keyring entry")?;

    entry.set_password(password)
        .context("Failed to save password to KWallet")?;
        
    Ok(())
}