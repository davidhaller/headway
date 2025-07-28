#include <QApplication>
#include <QTranslator>
#include <QQuickStyle>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "config.hpp"
#include "world.hpp"

static void configure(QApplication* application, QTranslator* translator, QQmlContext* context)
{
    application->setApplicationName("Headway");
    application->setApplicationVersion(Headway::VERSION);

    const QLocale locale = QLocale::system();
    const QLocale::Language language = locale.language();
    const QString languageCode = QLocale::languageToCode(language);

    if (!translator->load(languageCode, ":/i18n") && languageCode != "C" && languageCode != "en")
        qWarning() << "No translation available for " + QLocale::languageToString(language) + ", using English as fallback language.";
    else application->installTranslator(translator);

    context->setContextProperty("HEADWAY_VERSION", QVariant(Headway::VERSION));
    context->setContextProperty("HEADWAY_GLOBAL_MENUBAR", QVariant(Headway::GLOBAL_MENUBAR));
    QQuickStyle::setStyle(Headway::QUICK_CONTROLS_STYLE);

    qmlRegisterType<Headway::World>("Headway", Headway::VERSION_MAJOR, Headway::VERSION_MINOR, "World");
}

int main(int argc, char** argv)
{
    QApplication application(argc, argv);
    QTranslator translator;
    QQmlApplicationEngine engine;

    configure(&application, &translator, engine.rootContext());
    engine.load(QUrl(QStringLiteral("qrc:/MainWindow.qml")));

    if (engine.rootObjects().isEmpty())
        return EXIT_FAILURE;
    else return application.exec();
}
