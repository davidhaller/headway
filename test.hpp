#pragma once

#include <QtTest>

namespace Headway
{
    class Test : public QObject
    {
        Q_OBJECT

    private slots:
        void randomTooHigh();
        void cellWriteInvalid();
        void cellWriteTwice();
        void nextEq();
        void saveLoadEq();
    };
}
