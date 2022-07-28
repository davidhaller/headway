#include <QtCore>
#include <QtQml>
#include <QtQuickControls2>
#include <QtWidgets>

#include "config.hpp"
#include "world.hpp"

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    app.setApplicationName("Headway");

    const QLocale locale = QLocale::system();
    const QLocale::Language language = locale.language();
    const QString languageCode = QLocale::languageToCode(language);

    QTranslator translator;
    if (!translator.load(languageCode, ":/i18n") && languageCode != "C" && languageCode != "en")
        qWarning() << "No translation available for " + locale.languageToString(language) + ", using English as fallback language.";
    else app.installTranslator(&translator);

    QQuickStyle::setStyle(Headway::QUICK_CONTROLS_STYLE);

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("HEADWAY_VERSION_MAJOR", QVariant(Headway::VERSION_MAJOR));
    engine.rootContext()->setContextProperty("HEADWAY_VERSION_MINOR", QVariant(Headway::VERSION_MINOR));
    engine.rootContext()->setContextProperty("HEADWAY_GLOBAL_MENUBAR", QVariant(Headway::GLOBAL_MENUBAR));

    qmlRegisterType<Headway::World>("Headway", Headway::VERSION_MAJOR, Headway::VERSION_MINOR, "World");
    engine.load(QUrl(QStringLiteral("qrc:/MainWindow.qml")));

    if (engine.rootObjects().isEmpty())
        return EXIT_FAILURE;
    else return app.exec();
}
