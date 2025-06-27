#include "telemetrymodel.h"

TelemetryModel::TelemetryModel(QObject* parent)
    : QAbstractListModel(parent)
{}

int TelemetryModel::rowCount(const QModelIndex& parent) const {
    if (parent.isValid())
        return 0;
    return packets.size();
}

QVariant TelemetryModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid())
        return {};
    int row = index.row();
    if (row < 0 || row >= packets.size())
        return {};
    const Entry &e = packets.at(row);
    switch (role) {
    case TimestampRole:
        return e.timestamp.toString(Qt::ISODate);
    case HexRole:
        return e.data.toHex(' ').toUpper();
    case ChecksumRole: {
        quint8 sum = 0;
        for (auto b : e.data)
            sum += static_cast<quint8>(b);
        return QString::number(sum, 16).rightJustified(2, '0').toUpper();
    }
    default:
        return {};
    }
}

QHash<int, QByteArray> TelemetryModel::roleNames() const {
    return {
        { TimestampRole, "timestamp" },
        { HexRole,       "hex" },
        { ChecksumRole,  "checksum" }
    };
}

void TelemetryModel::appendPacket(const QByteArray& pkt, const QDateTime& ts) {
    beginInsertRows(QModelIndex(), packets.size(), packets.size());
    packets.append({pkt, ts});
    endInsertRows();
}
