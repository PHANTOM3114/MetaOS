use zbus::proxy;

#[proxy(
    interface = "org.ars.sonar.Interface",
    default_service = "org.ars.sonar",
    default_path = "/org/ars/sonar"
)]

pub trait SonarMessageAccept {
    #[zbus(signal)]
    async fn sonar_work_stop(&self, exit_message: &str) -> Result<()>;

    #[zbus(signal)]
    async fn sonar_crash(&self, crash_message: &str) -> Result<()>;
}