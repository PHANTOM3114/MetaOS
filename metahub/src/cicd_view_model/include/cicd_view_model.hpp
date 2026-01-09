#pragma once

#include <qcontainerfwd.h>
#include <qhash.h>
#include <qtypes.h>
#include <qvariant.h>
#include <QAbstractListModel>
#include <QDBusInterface>
#include <QDBusPendingCallWatcher>
#include <QDBusPendingReply>
#include <QDebug>
#include <QObject>
#include <Qt>

struct PipelineData {
    qint64 m_id;
    QString m_status;
    QString m_pipeline_name;
    QString m_conclusion;
};

enum eROLES { idRole = Qt::UserRole + 1, NameRole, StatusRole, ConclusionRole };

class CiCDViewModel : public QAbstractListModel {
    Q_OBJECT
public:
    explicit CiCDViewModel(QObject* parent = nullptr);

    virtual int rowCount(const QModelIndex& parent) const override { return m_pipline_data.size(); };
    virtual QVariant data(const QModelIndex& index, int role) const override {
        if (!index.isValid() || index.row() >= m_pipline_data.size()) {
            return QVariant();
        }

        const PipelineData& item = m_pipline_data[index.row()];

        switch (role) {
            case idRole:
                return item.m_id;
            case NameRole:
                return item.m_pipeline_name;
            case StatusRole:
                return item.m_status;
            case ConclusionRole:
                return item.m_conclusion;
        }

        return QVariant();
    };
    virtual QHash<int, QByteArray> roleNames() const override {
        QHash<int, QByteArray> roles;

        roles[idRole] = "id";
        roles[NameRole] = "name";
        roles[StatusRole] = "status";
        roles[ConclusionRole] = "conclusion";

        return roles;
    }

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