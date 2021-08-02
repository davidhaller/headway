#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QQuickStyle>
#include <QQmlApplicationEngine>

#include "world.hpp"

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    app.setApplicationName("Headway");

    const QLocale locale = QLocale::system();
    const QLocale::Language language = locale.language();
    const QString languageCode = locale.languageToCode(language);

    QTranslator translator;
    if (!translator.load(languageCode, ":/i18n") && languageCode != "C" && languageCode != "en")
        qWarning() << "No translation available for " + locale.languageToString(language) + ", using English as fallback language.";
    else app.installTranslator(&translator);

#ifdef Q_OS_WINDOWS
    QQuickStyle::setStyle("Universal");
#endif

    QQmlApplicationEngine engine;
    qmlRegisterType<Headway::World>("Headway", 4, 1, "World");
    engine.load(QUrl(QStringLiteral("qrc:/MainWindow.qml")));

    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
