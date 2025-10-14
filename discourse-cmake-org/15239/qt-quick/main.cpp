#include <QApplication>
#include <QQmlApplicationEngine>

#include "backend.h"

int main(int argc, char *argv[])
{
    #if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    #endif

    QApplication app(argc, argv);

    QQmlApplicationEngine engine;

    qmlRegisterType<Backend>("dev.decovar.Backend", 1, 0, "Backend");

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
