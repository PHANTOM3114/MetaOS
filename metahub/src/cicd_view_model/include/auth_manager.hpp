#include <qobject.h>
#include <qtmetamacros.h>

class QOAuth2AuthorizationCodeFlow;
class QOAuthHttpServerReplyHandler;

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

private slots:
    void onAuthFinished();

private:
    QOAuth2AuthorizationCodeFlow* m_oauth;
    QOAuthHttpServerReplyHandler* m_handler;
};