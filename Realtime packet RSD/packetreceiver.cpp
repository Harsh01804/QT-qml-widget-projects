#include "packetreceiver.h"
#include <QSerialPort>
#include <QDebug>

void PacketReceiver::run()
{
    qDebug() << "[INFO] PacketReceiver thread started";

    QSerialPort serial;
    serial.setPortName("COM6");  // 🔧 Make sure this is correct
    serial.setBaudRate(QSerialPort::Baud115200);
    serial.setDataBits(QSerialPort::Data8);
    serial.setParity(QSerialPort::NoParity);
    serial.setStopBits(QSerialPort::OneStop);

    if (!serial.open(QIODevice::ReadOnly)) {
        qWarning() << "[ERROR] Cannot open serial port:" << serial.errorString();
        return;
    }

    qDebug() << "[INFO] COM6 opened for reading";

    QByteArray current;
    bool inPacket = false;

    while (!isInterruptionRequested()) {
        if (!serial.waitForReadyRead(100))
            continue;

        QByteArray chunk = serial.readAll();
        if (!chunk.isEmpty())
            qDebug() << "[RAW]" << chunk.toHex(' ').toUpper();

        for (auto ub : chunk) {
            unsigned char b = static_cast<unsigned char>(ub);
            if (!inPacket) {
                if (b == HEADER_BYTE) {
                    inPacket = true;
                    current.clear();
                    current.append(static_cast<char>(b));
                }
            } else {
                current.append(static_cast<char>(b));

                if (current.size() == PACKET_SIZE) {
                    if (static_cast<unsigned char>(current.at(PACKET_SIZE - 1)) == FOOTER_BYTE) {
                        // ✅ Valid packet
                        QDateTime ts = QDateTime::currentDateTimeUtc();
                        buffer.push(current);
                        emit packetReceived(current, ts);

                        qDebug() << "[RECEIVED]" << ts.toString(Qt::ISODate)
                                 << "Length:" << current.size()
                                 << "Footer OK";
                        current.clear();
                        inPacket = false;
                    } else {
                        // ❌ Footer mismatch
                        qDebug() << "[INVALID PACKET] Footer mismatch. Data:" << current.toHex(' ').toUpper();

                        // Attempt resync by looking for new header
                        int newStart = -1;
                        for (int i = 1; i < PACKET_SIZE; ++i) {
                            if (static_cast<unsigned char>(current.at(i)) == HEADER_BYTE) {
                                newStart = i;
                                break;
                            }
                        }

                        if (newStart >= 0) {
                            QByteArray temp = current.mid(newStart);
                            current = temp;
                            inPacket = true;
                        } else {
                            current.clear();
                            inPacket = false;
                        }
                    }
                }
            }
        }
    }

    qDebug() << "[INFO] Application is quitting. Stopping receiver thread.";
    serial.close();
    buffer.stop();
}
