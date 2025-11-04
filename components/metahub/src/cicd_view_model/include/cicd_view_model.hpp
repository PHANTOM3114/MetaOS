#pragma once 

#include <QObject>
#include <QDebug>
#include <QDBusInterface>
#include <QDBusPendingCallWatcher>
#include <QDBusPendingReply>

class CiCDViewModel : public QObject
{
    Q_OBJECT
public:
    explicit CiCDViewModel(QObject *parent = nullptr);

signals:
    void cicdDataReceived(const QString &data);
    void cicdErrorReceived(const QString &error);

public slots:
    void fetchCicdData();

private slots:
    void onDataFetchFinished(QDBusPendingCallWatcher *watcher);

private:
    QDBusInterface *m_dbusInterface;
};