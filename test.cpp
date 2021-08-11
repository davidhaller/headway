#include "world.hpp"
#include "test.hpp"

using namespace Headway;

void Test::randomTooHigh()
{
    World biotope;
    biotope.createWorld(2, 2);

    bool success = biotope.random(50000);
    QVERIFY(!success);
}

void Test::cellWriteInvalid()
{
    World biotope;
    biotope.createWorld(10, 11);

    QVERIFY(!biotope.isValid(10, 10));
    QVERIFY(!biotope.createCell(10, 10));
}

void Test::cellWriteTwice()
{
    World biotope;
    biotope.createWorld(10, 15);
    biotope.createCell(1, 2);

    QVERIFY(!biotope.createCell(1, 2));
}

void Test::nextEq()
{
    World w1;
    w1.createWorld(40, 30, 2);
    w1.random(100);

    World w2(w1);
    w1.next();

    QVERIFY(w1 != w2);

    w2.next();

    QVERIFY(w1 == w2);
}

void Test::saveLoadEq()
{
    World genesis;
    genesis.createWorld(8, 20, 4);
    genesis.random(10);

    QTemporaryFile xmlFile(QDir::temp().filePath("XXXXXX.xml")); xmlFile.open();
    QTemporaryFile jsonFile(QDir::temp().filePath("XXXXXX.json")); jsonFile.open();

    const QUrl xmlUrl = QUrl::fromLocalFile(xmlFile.fileName());
    const QUrl jsonUrl = QUrl::fromLocalFile(jsonFile.fileName());

    QVERIFY(genesis.saveFile(xmlUrl));
    QVERIFY(genesis.saveFile(jsonUrl));

    World xmlWorld;
    QVERIFY(xmlWorld.loadFile(xmlUrl));

    World jsonWorld;
    QVERIFY(jsonWorld.loadFile(jsonUrl));

    QVERIFY(genesis == xmlWorld);
    QVERIFY(genesis == jsonWorld);

    xmlFile.close();
    jsonFile.close();
}

QTEST_MAIN(Headway::Test)
