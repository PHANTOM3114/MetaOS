#pragma once

#include <qabstractoauth.h>
#include <qdebug.h>
#include <qnetworkaccessmanager.h>
#include <qnetworkreply.h>
#include <qnetworkrequest.h>
#include <qobject.h>
#include <qtmetamacros.h>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

namespace Ars::Deck {
class JsonNetworkManager : public QNetworkAccessManager {
    Q_OBJECT
public:
    explicit JsonNetworkManager(QObject* parent = nullptr) : QNetworkAccessManager(parent) {}

protected:
    QNetworkReply* createRequest(Operation operation, const QNetworkRequest& request, QIODevice* outgoingData = nullptr) override {
        QNetworkRequest newRequest = request;

        newRequest.setRawHeader("Accept", "application/json");
        return QNetworkAccessManager::createRequest(operation, newRequest, outgoingData);
    }
};
}  // namespace Ars::Deck
