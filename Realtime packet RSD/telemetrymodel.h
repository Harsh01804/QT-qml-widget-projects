#pragma once

#include <QAbstractListModel>
#include <QList>
#include <QByteArray>
#include <QDateTime>

class TelemetryModel : public QAbstractListModel {
    Q_OBJECT

public:
    enum Roles {
        TimestampRole = Qt::UserRole + 1,
        HexRole,
        ChecksumRole
    };

    explicit TelemetryModel(QObject* parent = nullptr);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE void appendPacket(const QByteArray& pkt, const QDateTime& ts);

private:
    struct Entry {
        QByteArray data;
        QDateTime timestamp;
    };
    QList<Entry> packets;
};
