QT += core xml qml quick testlib
CONFIG += c++14

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
    CONFIG += console testcase
    TARGET = headway-test

    SOURCES += tests/main_test.cpp \
        tests/world_test.cpp

    HEADERS += tests/world_test.hpp
} else {
    TARGET = headway

    SOURCES += main.cpp
    RESOURCES += qml.qrc
}

DEFINES += QT_DEPRECATED_WARNINGS
