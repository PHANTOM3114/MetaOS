#include "auth_manager.hpp"
#include <QDBusReply>
#include <QDebug>
#include <QDesktopServices>
#include <QOAuth2AuthorizationCodeFlow>
#include <QOAuthHttpServerReplyHandler>
#include <QUrl>

const QString SONAR_SERVICE_NAME = "org.ars.sonar";
const QString SONAR_OBJECT_PATH = "/org/ars/sonar";
const QString SONAR_INTERFACE_NAME = "org.ars.sonar.Interface";

namespace Ars::Deck {
AuthManager::AuthManager(QObject* parent)
    : QObject(parent), m_oauth(new QOAuth2AuthorizationCodeFlow(this)), m_handler(new QOAuthHttpServerReplyHandler(8080, this)) {
    m_dbusInterface = new QDBusInterface(SONAR_SERVICE_NAME, SONAR_OBJECT_PATH, SONAR_INTERFACE_NAME, QDBusConnection::sessionBus(), this);

    if (!m_dbusInterface->isValid()) {
        qWarning() << "D-Bus interface is NOT valid:" << SONAR_SERVICE_NAME;
    }
    this->setupGithub();

    m_oauth->setReplyHandler(m_handler);

    qInfo() << "Generated Callback URL:" << m_handler->callback();

    connect(m_oauth, &QOAuth2AuthorizationCodeFlow::authorizeWithBrowser, &QDesktopServices::openUrl);
    connect(m_oauth, &QOAuth2AuthorizationCodeFlow::granted, this, &AuthManager::onAuthFinished);

    connect(m_oauth, &QOAuth2AuthorizationCodeFlow::requestFailed, this, [](const QAbstractOAuth::Error error) {
        //handling errors
        qInfo() << "Error!";
    });
}

void AuthManager::startAuth() {
    if (m_oauth->clientIdentifier().isEmpty()) {
        qWarning() << "Cannot start auth: Client ID is not set";
        emit tokenErrorReceived("Client ID missing");
        return;
    }
    m_oauth->grant();
}

void AuthManager::setupGithub() {
    m_oauth->setAuthorizationUrl(QUrl("https://github.com/login/oauth/authorize"));
    m_oauth->setTokenUrl(QUrl("https://github.com/login/oauth/access_token"));
    m_oauth->setClientIdentifier("Ov23liaSalefAhl16gjU");
    m_oauth->setClientIdentifierSharedKey("REPLACE_ME");
    m_oauth->setRequestedScopeTokens({"repo", "user"});
}

void AuthManager::setupGitlab() {
    // Not implemented yet
}

void AuthManager::onAuthFinished() {
    if (m_oauth->status() == QAbstractOAuth::Status::Granted) {
        QString token = m_oauth->token();
        qInfo() << "Authentication Successful!";
        qInfo() << "Access Token:" << token;

        QDBusReply<bool> reply = m_dbusInterface->call("UpdateToken", "github", token);
        if (reply.isValid() && reply.value()) {
            qInfo() << "Token successfully sent to daemon";
        } else {
            qWarning() << "Failed to send token: " << reply.error().message();
        }
        emit tokenReceived(token);
    } else {
        qWarning() << "Auth finished but status is not Granted";
        emit tokenErrorReceived("Auth failed or rejected");
    }
}

}  // namespace Ars::Deck
