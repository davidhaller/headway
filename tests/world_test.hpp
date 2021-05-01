#pragma once

#include <QtCore>
#include <QtTest>

namespace Headway
{
    class TestWorld : public QObject
    {
        Q_OBJECT

    private slots:
        void testRandomTooHigh();
        void testCellWriteInvalid();
        void testCellWriteTwice();
        void testNextEq();
        void testSaveLoadEq();

        // TODO check backup, load invalid file?
    };
}
