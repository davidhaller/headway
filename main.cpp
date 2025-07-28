#include <QApplication>
#include <QTranslator>
#include <QQmlApplicationEngine>

#include "world.hpp"

static void configure(QApplication* application, QTranslator* translator)
{
    application->setApplicationName("Headway");
    application->setApplicationVersion(QString::number(HEADWAY_VERSION));

    const QLocale locale = QLocale::system();
    const QLocale::Language language = locale.language();
    const QString languageCode = QLocale::languageToCode(language);

    if (!translator->load(languageCode, ":/i18n") && languageCode != "C" && languageCode != "en")
        qWarning() << "No translation available for " + QLocale::languageToString(language) + ", using English as fallback language.";
    else application->installTranslator(translator);

    qmlRegisterType<Headway::World>("Headway", HEADWAY_VERSION_MAJOR, HEADWAY_VERSION_MINOR, "World");
}

int main(int argc, char** argv)
{
    QApplication application(argc, argv);
    QTranslator translator;
    QQmlApplicationEngine engine;

    configure(&application, &translator);
    engine.loadFromModule("io.github.davidhaller.headway", "MainWindow");

    if (engine.rootObjects().isEmpty())
        return EXIT_FAILURE;
    else return application.exec();
}
