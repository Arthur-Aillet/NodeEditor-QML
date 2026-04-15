cmake_minimum_required(VERSION 3.16)

project(CutieDesigner VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_PREFIX_PATH "~/Qt/6.11.0/gcc_64/")
set(QT_QML_GENERATE_QMLLS_INI ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Qt6 REQUIRED COMPONENTS Quick Core)

qt_standard_project_setup()

set(QT_QML_GENERATE_QMLLS_INI ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#set(CMAKE_CXX_CLANG_TIDY clang-tidy-19 -config-file=${CMAKE_SOURCE_DIR}/.clang-tidy)

option(DEBUG_MODE "Set DEBUG Mode" ON)
option(RUN "Launch App" OFF)

add_subdirectory(external/nodeeditor)

add_subdirectory(src)

if(RUN)
    add_custom_target(run
        COMMENT "Launching binary"
        COMMAND CutieDesigner
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
endif()
