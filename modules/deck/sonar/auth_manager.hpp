#include <QDBusConnection>
#include <QDBusInterface>
#include <QDBusPendingCallWatcher>
#include <QDBusPendingReply>

class QOAuth2DeviceAuthorizationFlow;

namespace Ars {
namespace Deck {
class AuthManager : public QObject {
    Q_OBJECT

public:
    explicit AuthManager(QObject* parent = nullptr);

public slots:
    void startAuth();
    void setupGithub();
    void setupGitlab();

signals:
    void tokenReceived(const QString& token);
    void tokenErrorReceived(const QString& error);

    void deviceAuthReady(const QString& url, const QString& userCode);

private slots:
    void onAuthFinished();

private:
    QOAuth2DeviceAuthorizationFlow* m_oauth;
    QDBusInterface* m_dbusInterface;
};
}  // namespace Deck
}  // namespace Ars
