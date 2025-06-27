#include "filelogger.h"

FileLogger::FileLogger(const QString& filename, QObject* parent)
    : QObject(parent), logFile(filename), csvFile(filename + ".csv") {
    if (!logFile.open(QIODevice::Append | QIODevice::Text))
        qWarning() << "Could not open log file" << filename;
    if (!csvFile.open(QIODevice::Append | QIODevice::Text))
        qWarning() << "Could not open CSV file" << csvFile.fileName();

    QTextStream csvOut(&csvFile);
    csvOut << "idx";
    for (int i = 0; i < 128; ++i) csvOut << ",b" << i;
    csvOut << ",activity,checksum,footer\n";
}

void FileLogger::logPacket(const QByteArray& packet) {
    QString timestamp = QDateTime::currentDateTimeUtc().toString(Qt::ISODate);
    quint8 checksum = 0;
    for (auto b : packet) checksum += static_cast<quint8>(b);
    QTextStream out(&logFile);
    out << timestamp
        << " | CHK=0x" << QString::number(checksum, 16).rightJustified(2, '0').toUpper()
        << " | DATA=" << packet.toHex().toUpper() << "\n";
    out.flush();

    static quint64 idx = 0;
    ++idx;
    QTextStream csvOut(&csvFile);
    csvOut << idx;
    for (int i = 0; i < 128; ++i)
        csvOut << "," << QString::number(static_cast<quint8>(packet[i]));
    csvOut << "," << QString::number(static_cast<quint8>(packet[54]))
           << "," << QString::number(static_cast<quint8>(packet[55]))
           << "," << QString::number(static_cast<quint8>(packet[127])) << "\n";
    csvOut.flush();
}
