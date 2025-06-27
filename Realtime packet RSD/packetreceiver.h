#pragma once
#include <QThread>
#include <QByteArray>
#include <QDateTime>
#include "CircularBuffer.h"

class PacketReceiver : public QThread {
    Q_OBJECT
    Q_PROPERTY(bool isRunning READ isRunning NOTIFY runningChanged)

public:
    explicit PacketReceiver(CircularBuffer<QByteArray>& buf, QObject* parent = nullptr)
        : QThread(parent), buffer(buf), m_running(false) {}

    bool isRunning() const { return m_running; }

signals:
    void packetReceived(const QByteArray& packet, const QDateTime& timestamp);
    void runningChanged();

public slots:
    void startReceiving() {
        if (!m_running) {
            m_running = true;
            emit runningChanged();
            QThread::start();
        }
    }

    void stopReceiving() {
        if (m_running) {
            requestInterruption();
            wait();
            buffer.stop();
            m_running = false;
            emit runningChanged();
        }
    }

protected:
    void run() override;

private:
    CircularBuffer<QByteArray>& buffer;
    bool m_running;
    static constexpr unsigned char HEADER_BYTE = 0xAA;
    static constexpr unsigned char FOOTER_BYTE = 0xEE;
    static const int PACKET_SIZE = 128;
};
