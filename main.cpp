#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>

#include "world.hpp"

int main(int argc, char** argv)
{
    QApplication app(argc, argv);

#ifdef Q_OS_WINDOWS
    QQuickStyle::setStyle("Universal");
#else
    QQuickStyle::setStyle("Basic");
#endif

    QQmlApplicationEngine engine;
    qmlRegisterType<Headway::World>("Headway", 4, 1, "World");
    engine.load(QUrl(QStringLiteral("qrc:/MainWindow.qml")));

    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
