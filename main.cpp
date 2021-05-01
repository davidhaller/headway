#include <QtWidgets/QApplication>
#include <QtQml/QQmlApplicationEngine>

#include "world.hpp"

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    QQmlApplicationEngine engine;

    qmlRegisterType<Headway::World>("Headway", 4, 1, "World");
    engine.load(QUrl(QStringLiteral("qrc:/MainWindow.qml")));

    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
