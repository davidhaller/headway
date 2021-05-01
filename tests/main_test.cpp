#include <QtCore>
#include <QtTest>

#include "tests/world_test.hpp"

int main(int argc, char** argv)
{
    QCoreApplication app(argc, argv);
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

    Headway::TestWorld testworld;
    return QTest::qExec(&testworld, argc, argv);
}
