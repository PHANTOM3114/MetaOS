// components/metahub/src/cicd_manager.cpp

#include "cicd_view_model.hpp"

const QString CICD_SERVICE_NAME = "org.metaos.CiCdService";
const QString CICD_OBJECT_PATH = "/org/metaos/CiCd";
const QString CICD_INTERFACE_NAME = "org.metaos.CiCd.Interface";

CiCDViewModel::CiCDViewModel(QObject *parent) : QObject(parent)
{
    m_dbusInterface = new QDBusInterface(
        CICD_SERVICE_NAME,
        CICD_OBJECT_PATH,
        CICD_INTERFACE_NAME,
        QDBusConnection::sessionBus(),
        this
    );

    if (!m_dbusInterface->isValid()) {
        qWarning() << "D-Bus interface is NOT valid:" << CICD_SERVICE_NAME;
    }
}

void CiCDViewModel::fetchCicdData()
{
    qInfo() << "QML button clicked, calling D-Bus method FetchData...";

    QDBusPendingCall asyncCall = m_dbusInterface->asyncCall("PipelineStatusFetch");
    QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(asyncCall, this);
    
    connect(watcher, &QDBusPendingCallWatcher::finished, this, &CiCDViewModel::onDataFetchFinished);
}

void CiCDViewModel::onDataFetchFinished(QDBusPendingCallWatcher *watcher)
{
    QDBusPendingReply<QString> reply = *watcher;

    if (reply.isValid()) {
        QString result = reply.value();
        qInfo() << "D-Bus Reply OK:" << result;
        emit cicdDataReceived(result);
    } 
    else {
        QString error = reply.error().message();
        qWarning() << "D-Bus Call Failed:" << error;
        emit cicdErrorReceived(error); 
    }

    watcher->deleteLater();
}