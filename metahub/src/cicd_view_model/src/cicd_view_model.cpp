// components/metahub/src/cicd_manager.cpp

#include "cicd_view_model.hpp"
#include <qcontainerfwd.h>
#include <qlogging.h>
#include <qtdbusglobal.h>
#include <qtypes.h>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QVariant>

const QString CICD_SERVICE_NAME = "org.metaos.CiCdService";
const QString CICD_OBJECT_PATH = "/org/metaos/CiCd";
const QString CICD_INTERFACE_NAME = "org.metaos.CiCd.Interface";

CiCDViewModel::CiCDViewModel(QObject* parent) {
    m_dbusInterface = new QDBusInterface(CICD_SERVICE_NAME, CICD_OBJECT_PATH, CICD_INTERFACE_NAME, QDBusConnection::sessionBus(), this);

    if (!m_dbusInterface->isValid()) {
        qWarning() << "D-Bus interface is NOT valid:" << CICD_SERVICE_NAME;
    }
}

void CiCDViewModel::fetchCicdData() {
    qInfo() << "QML button clicked, calling D-Bus method FetchData...";

    QDBusPendingCall asyncCall = m_dbusInterface->asyncCall("PipelineStatusFetch");
    QDBusPendingCallWatcher* watcher = new QDBusPendingCallWatcher(asyncCall, this);

    connect(watcher, &QDBusPendingCallWatcher::finished, this, &CiCDViewModel::onDataFetchFinished);
}

void CiCDViewModel::onDataFetchFinished(QDBusPendingCallWatcher* watcher) {
    QDBusPendingReply<QString> reply = *watcher;

    if (reply.isValid()) {
        QByteArray result = reply.value().toUtf8();
        qInfo() << "D-Bus Reply OK:";
        qInfo() << "RAW JSON:" << result;

        beginResetModel();
        m_pipline_data.clear();

        QJsonDocument doc;
        doc = QJsonDocument::fromJson(result);

        if (!doc.isArray()) {
            qWarning() << "Unvalid Json";
            endResetModel();
            return;
        } else {
            QJsonArray json_array = doc.array();

            for (auto pipeline : json_array) {
                PipelineData pipeline_data;
                QJsonObject object = pipeline.toObject();

                pipeline_data.m_id = object["id"].toInteger();
                pipeline_data.m_pipeline_name = object["name"].toString();
                pipeline_data.m_status = object["status"].toString();
                pipeline_data.m_conclusion = object["conclusion"].toString();

                m_pipline_data.push_back(pipeline_data);
            }
        }
        endResetModel();

        emit cicdDataReceived(result);
    } else {
        QString error = reply.error().message();
        qWarning() << "D-Bus Call Failed:" << error;
        emit cicdErrorReceived(error);
    }

    watcher->deleteLater();
}