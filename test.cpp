#include "world.hpp"
#include "test.hpp"

using namespace Headway;

void Test::randomTooHigh()
{
    World biotope;
    biotope.createWorld(2, 2);
    QSignalSpy spy(&biotope, &World::error);

    QVERIFY(!biotope.random(50000));
    QCOMPARE(spy.count(), 1);
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

    QCOMPARE(xmlWorld, genesis);
    QCOMPARE(jsonWorld, genesis);

    xmlFile.close();
    jsonFile.close();
}

void Test::loadInvalid()
{
    QTemporaryFile invalidFile(QDir::temp().filePath("XXXXXX.xml"));
    invalidFile.open();

    QTextStream stream(&invalidFile);
    stream << "Lorem ipsum";
    stream.flush();

    const QUrl fileUrl = QUrl::fromLocalFile(invalidFile.fileName());
    World invalidWorld;
    QSignalSpy spy(&invalidWorld, &World::error);

    QVERIFY(!invalidWorld.loadFile(fileUrl));
    QCOMPARE(spy.count(), 1);

    invalidFile.close();
}

void Test::createInvalid()
{
    World biotope;
    QSignalSpy spy(&biotope, &World::error);

    QVERIFY(!biotope.createWorld(0, 2));
    QCOMPARE(spy.count(), 1);
}

QTEST_MAIN(Headway::Test)
