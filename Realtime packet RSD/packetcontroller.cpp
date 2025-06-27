#include "packetcontroller.h"
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QDebug>

PacketController::PacketController(CircularBuffer<QByteArray>& buf,
                                   FileLogger&                logger,
                                   const QString&             defaultCsv,
                                   QObject*                   parent)
    : QObject(parent)
    , m_buf(buf)
    , m_logger(logger)
    , m_defaultCsv(defaultCsv)
{}


bool PacketController::exportCsvAndLog()
{
    QFile file(m_defaultCsv);
    const bool newFile = !file.exists();

    if (!file.open(QIODevice::Append | QIODevice::Text)) {
        qWarning() << "Cannot open CSV file:" << m_defaultCsv;
        return false;
    }
    QTextStream out(&file);

    if (newFile && !ensureCsvHeader(out))
        return false;

    while (!m_buf.empty()) {
        const QByteArray pkt = m_buf.pop();
        if (pkt.isEmpty()) break;

        /* ---- CSV ---- */
        const QString ts = QDateTime::currentDateTime().toString(Qt::ISODate);
        out << ts;
        for (int i = 0; i < 128; ++i) {
            QString hv;
            if (i < pkt.size()) {
                const quint8 b = static_cast<quint8>(pkt[i]);
                hv = QString::number(b, 16).rightJustified(2, '0').toUpper();
            }
            out << ',' << hv;
        }
        out << '\n';

        /* ---- LOG ---- */
        m_logger.logPacket(pkt);
    }
    return true;
}

bool PacketController::ensureCsvHeader(QTextStream& out)
{
    out << "Timestamp";
    for (int i = 0; i < 128; ++i) out << ",Byte" << i;
    out << '\n';
    return true;
}

QString PacketController::readCsv(const QString& fileIn) const
{
    const QString fname = fileIn.isEmpty() ? m_defaultCsv : fileIn;
    QFile file(fname);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return QString("[ERROR] %1").arg(file.errorString());
    return QString::fromUtf8(file.readAll());
}
