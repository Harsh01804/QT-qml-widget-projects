

#include "camera.h"

#include <QApplication>

int main(int argc, char *argv[])
{
#ifdef Q_OS_ANDROID




    QCoreApplication::setAttribute(Qt::AA_DontUseNativeMenuBar);
#endif
    QApplication app(argc, argv);

    Camera camera;
    camera.show();

    return app.exec();
};
