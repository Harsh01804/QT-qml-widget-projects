#pragma once
#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QDebug>

// Logs each packet with UTC timestamp, checksum, hex data.
class FileLogger : public QObject {
    Q_OBJECT
public:
    explicit FileLogger(const QString& filename, QObject* parent = nullptr)
        : QObject(parent), logFile(filename)
    {
        if (!logFile.open(QIODevice::Append | QIODevice::Text)) {
            qWarning() << "Could not open log file" << filename;
        }
    }

public slots:
    void logPacket(const QByteArray& packet) {
        QString ts = QDateTime::currentDateTimeUtc().toString(Qt::ISODate);
        quint8 chk = 0;
        for (auto b : packet) chk += static_cast<quint8>(b);
        QTextStream out(&logFile);
        out << ts
            << " | CHK=0x" << QString::number(chk, 16).rightJustified(2, '0').toUpper()
            << " | DATA=" << packet.toHex().toUpper() << "\n";
        out.flush();
    }

private:
    QFile logFile;
};
