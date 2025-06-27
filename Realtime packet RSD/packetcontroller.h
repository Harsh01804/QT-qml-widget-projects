#pragma once
#include <QObject>
#include "CircularBuffer.h"
#include "FileLogger.h"

class PacketController : public QObject {
    Q_OBJECT
public:
    PacketController(CircularBuffer<QByteArray>& buf,
                     FileLogger&                logger,
                     const QString&             defaultCsv,
                     QObject*                   parent = nullptr);

    Q_INVOKABLE bool exportCsvAndLog();          
    Q_INVOKABLE QString defaultCsv() const { return m_defaultCsv; }
    Q_INVOKABLE QString readCsv(const QString& file = {}) const;

private:
    bool ensureCsvHeader(QTextStream& out);

    CircularBuffer<QByteArray>& m_buf;
    FileLogger&                 m_logger;
    QString                     m_defaultCsv;
};
