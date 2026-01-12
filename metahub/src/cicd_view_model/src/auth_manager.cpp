#include "auth_manager.hpp"
#include <QDebug>
#include <QDesktopServices>
#include <QOAuth2AuthorizationCodeFlow>
#include <QOAuthHttpServerReplyHandler>
#include <QUrl>

AuthManager::AuthManager(QObject* parent)
    : QObject(parent), m_oauth(new QOAuth2AuthorizationCodeFlow(this)), m_handler(new QOAuthHttpServerReplyHandler(8080, this)) {

    this->setupGithub();

    m_oauth->setReplyHandler(m_handler);

    connect(m_oauth, &QOAuth2AuthorizationCodeFlow::authorizeWithBrowser, &QDesktopServices::openUrl);
    connect(m_oauth, &QOAuth2AuthorizationCodeFlow::granted, this, &AuthManager::onAuthFinished);

    connect(m_oauth, &QOAuth2AuthorizationCodeFlow::requestFailed, this, [](const QAbstractOAuth::Error error) {
        //handling errors
        qInfo() << "Error!";
    });
}

void AuthManager::setupGithub() {
    m_oauth->setAuthorizationUrl(QUrl("https://github.com/login/oauth/authorize"));
    m_oauth->setTokenUrl(QUrl("https://github.com/login/oauth/access_token"));
    m_oauth->setClientIdentifier("Ov23liaSalefAhl16gjU");
    m_oauth->setClientIdentifierSharedKey("REPLACE_ME");
    m_oauth->setRequestedScopeTokens({"repo", "user"});
}
