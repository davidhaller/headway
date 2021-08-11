QT += core xml qml quick quickcontrols2 widgets
CONFIG += c++20 lrelease embed_translations

DEFINES += QT_DEPRECATED_WARNINGS QT_DISABLE_DEPRECATED_BEFORE=0x060000

SOURCES += \
    xmlfile.cpp \
    jsonfile.cpp \
    world.cpp \

HEADERS += \
    file.hpp \
    matrix.hpp \
    xmlfile.hpp \
    jsonfile.hpp \
    world.hpp

testconfig {
    QT += testlib

    CONFIG += console testcase
    CONFIG -= app_bundle

    TARGET = headway-test

    SOURCES += test.cpp
    HEADERS += test.hpp
} else {
    TARGET = Headway

    SOURCES += main.cpp
    RESOURCES += qml.qrc
    TRANSLATIONS = i18n/de.ts
}
