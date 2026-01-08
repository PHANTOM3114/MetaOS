#pragma once

#include <qcontainerfwd.h>
#include <qtypes.h>
#include <QDBusInterface>
#include <QDBusPendingCallWatcher>
#include <QDBusPendingReply>
#include <QDebug>
#include <QObject>

struct PipelineData {
    qint64 m_id;
    QString m_status;
    QString m_pipeline_name;
    QString m_conclusion;
};

class CiCDViewModel : public QObject {
    Q_OBJECT
public:
    explicit CiCDViewModel(QObject* parent = nullptr);

signals:
    void cicdDataReceived(const QString& data);
    void cicdErrorReceived(const QString& error);

public slots:
    void fetchCicdData();

private slots:
    void onDataFetchFinished(QDBusPendingCallWatcher* watcher);

private:
    QDBusInterface* m_dbusInterface;
    QVector<PipelineData> m_pipline_data;
};