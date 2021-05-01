cmake_minimum_required(VERSION 3.8)
project(headway CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -O3 -fPIE -fstack-protector-strong -pthread -Wall -Werror -Wextra")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pie -Wl,-z,now -Wl,-z,relro")

find_package(PkgConfig REQUIRED)

pkg_check_modules(GLIB REQUIRED glibmm-2.4)
pkg_check_modules(GTK REQUIRED gtkmm-3.0)
pkg_check_modules(GOOCANVAS REQUIRED goocanvasmm-2.0)
pkg_check_modules(SIGC REQUIRED sigc++-2.0)
pkg_check_modules(LIBXML REQUIRED libxml++-3.0)
pkg_check_modules(JSON REQUIRED jsoncpp)

include_directories(${GLIB_INCLUDE_DIRS}
                    ${GTK_INCLUDE_DIRS}
                    ${GOOCANVAS_INCLUDE_DIRS}
                    ${SIGC_INCLUDE_DIRS}
                    ${LIBXML_INCLUDE_DIRS}
                    ${JSON_INCLUDE_DIRS})

set(SOURCE_FILES src/app.cpp src/app.hpp
                 src/window.cpp src/window.hpp
                 src/world.cpp src/world.hpp src/matrix.hpp
                 src/json_file.cpp src/json_file.hpp
                 src/xml_file.cpp src/xml_file.hpp src/file.hpp)

add_library(headway-modules ${SOURCE_FILES})
target_link_libraries(headway-modules ${GLIB_LIBRARIES} ${GTK_LIBRARIES}
        ${GOOCANVAS_LIBRARIES} ${SIGC_LIBRARIES} ${LIBXML_LIBRARIES} ${JSON_LIBRARIES})

add_executable(headway src/main.cpp)
target_link_libraries(headway headway-modules)

install(TARGETS headway RUNTIME DESTINATION bin)
install(FILES ${CMAKE_SOURCE_DIR}/LICENSE DESTINATION ${CMAKE_INSTALL_PREFIX}/share/licenses/headway)
install(FILES ${CMAKE_SOURCE_DIR}/README.md DESTINATION ${CMAKE_INSTALL_PREFIX}/share/doc/headway)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/examples DESTINATION ${CMAKE_INSTALL_PREFIX}/share/doc/headway)

find_package(GTest)
if(GTEST_FOUND)
    include_directories(${GTEST_INCLUDE_DIRS})
    set(TEST_FILES src/world_test.cpp)
    add_executable(headway-test ${TEST_FILES})
    target_link_libraries(headway-test headway-modules ${GTEST_BOTH_LIBRARIES})

    enable_testing()
    add_test(headway headway-test)
endif()

find_package(Doxygen)
if(DOXYGEN_FOUND)
    set(DOC_DIR ${CMAKE_SOURCE_DIR}/doc)
    file(MAKE_DIRECTORY ${DOC_DIR})
    configure_file(${CMAKE_SOURCE_DIR}/Doxyfile.in ${CMAKE_BINARY_DIR}/Doxyfile @ONLY)
    add_custom_target(doc ${DOXYGEN_EXECUTABLE} ${CMAKE_BINARY_DIR}/Doxyfile
                      WORKING_DIRECTORY ${DOC_DIR} COMMENT "Generating API documentation with Doxygen" VERBATIM)
endif()
