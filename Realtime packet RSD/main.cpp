#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QDateTime>

#include "CircularBuffer.h"
#include "packetreceiver.h"
#include "packetcontroller.h"
#include "telemetrymodel.h"
#include "FileLogger.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    const QString stamp =
        QDateTime::currentDateTime().toString("dd-MM-yyyy_HH-mm-ss");

    CircularBuffer<QByteArray> buffer(1024);

    const QString logName = QString("telemetry_%1.log").arg(stamp);
    const QString csvName = QString("packets_%1.csv").arg(stamp);

    FileLogger       logger(logName);
    PacketController controller(buffer, logger, csvName);
    PacketReceiver   receiver(buffer);
    TelemetryModel   model;

    QObject::connect(&receiver, &PacketReceiver::packetReceived,
                     &model,    &TelemetryModel::appendPacket,
                     Qt::QueuedConnection);

    QQmlApplicationEngine engine;
    engine.addImportPath("E:/Qt/6.9.1/mingw_64/qml");   

    engine.rootContext()->setContextProperty("telemetryModel", &model);
    engine.rootContext()->setContextProperty("controller",     &controller);
    engine.rootContext()->setContextProperty("receiver",       &receiver);
    engine.rootContext()->setContextProperty("logger",         &logger);
    engine.rootContext()->setContextProperty("runCsv",         csvName);

    engine.loadFromModule("telback", "Main");
    if (engine.rootObjects().isEmpty())
        return -1;

    receiver.start();
    QObject::connect(&app, &QCoreApplication::aboutToQuit, [&]{
        receiver.requestInterruption();
        receiver.wait();
        buffer.stop();
    });

    return app.exec();
}
