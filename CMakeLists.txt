cmake_minimum_required(VERSION 3.16)

project(Headway VERSION 4.2 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt6 REQUIRED COMPONENTS Core LinguistTools Quick Gui Qml Quick QuickControls2 Test Widgets Xml)
qt_standard_project_setup(REQUIRES 6.9)

set(HEADWAY_CXX_FILES
    file.hpp
    jsonfile.cpp
    jsonfile.hpp
    xmlfile.hpp
    xmlfile.cpp
    matrix.hpp
    world.hpp
    world.cpp
)

qt_add_library(HeadwayCore STATIC ${HEADWAY_CXX_FILES})
qt_add_executable(Headway main.cpp)

set(HEADWAY_QML_FILES
    AboutDialog.qml
    Cell.qml
    CellCanvas.qml
    ControlToolBar.qml
    CreateRandomCellsDialog.qml
    CreateWorldDialog.qml
    MainWindow.qml
)

set(HEADWAY_PROJECT_URI io.github.davidhaller.headway)

qt_add_qml_module(Headway
    URI ${HEADWAY_PROJECT_URI}
    VERSION ${PROJECT_VERSION}
    QML_FILES ${HEADWAY_QML_FILES}
)

set_target_properties(Headway PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER ${HEADWAY_PROJECT_URI}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(HeadwayCore PUBLIC
    Qt::Core
    Qt::Gui
    Qt::Qml
    Qt::Quick
    Qt::QuickControls2
    Qt::Widgets
    Qt::Xml
)
target_link_libraries(Headway PRIVATE HeadwayCore)
target_compile_definitions(Headway PRIVATE
    HEADWAY_VERSION=${PROJECT_VERSION}
    HEADWAY_VERSION_MAJOR=${PROJECT_VERSION_MAJOR}
    HEADWAY_VERSION_MINOR=${PROJECT_VERSION_MINOR})
qt_add_translations(Headway TS_FILES i18n/de.ts)

#include(GNUInstallDirs)
install(TARGETS Headway
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

enable_testing()
add_executable(HeadwayTest test.hpp test.cpp)
target_link_libraries(HeadwayTest PRIVATE Qt::Test HeadwayCore)
add_test(NAME HeadwayTest COMMAND HeadwayTest)
