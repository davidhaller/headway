#include <QApplication>
#include <QTranslator>
#include <QQmlApplicationEngine>

#include "world.hpp"

int main(int argc, char** argv)
{
    QApplication application(argc, argv);
    application.setApplicationName(HEADWAY_NAME);
    application.setApplicationVersion(HEADWAY_VERSION);

    const QLocale locale = QLocale::system();
    const QLocale::Language language = locale.language();
    const QString languageCode = QLocale::languageToCode(language);

    QTranslator translator;
    if (!translator.load(languageCode, ":/i18n") && languageCode != "C" && languageCode != "en")
        qWarning() << "No translation available for " + QLocale::languageToString(language) + ", using English as fallback language.";
    else application.installTranslator(&translator);

    qmlRegisterType<Headway::World>(HEADWAY_NAME, HEADWAY_VERSION_MAJOR, HEADWAY_VERSION_MINOR, "World");
    QQmlApplicationEngine engine;
    engine.loadFromModule(HEADWAY_URI, "MainWindow");

    if (engine.rootObjects().isEmpty())
        return EXIT_FAILURE;
    else return application.exec();
}
