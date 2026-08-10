cmake_minimum_required(VERSION 3.10..4.0)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)

set(FETCHCONTENT_BASE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(CutieDesigner VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

list(APPEND CMAKE_PREFIX_PATH "~/Qt/6.11.0/gcc_64/lib/cmake")
# set(CMAKE_PREFIX_PATH "/usr/lib/x86_64-linux-gnu/")
set(LD_LIBRARY_PATH "~/Qt/6.11.0/gcc_64/lib")
set(QT_QML_GENERATE_QMLLS_INI ON)

cmake_policy(SET CMP0071 NEW)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Quick Qml ShaderTools)

qt_policy(SET QTP0001 NEW)
qt_policy(SET QTP0004 NEW)


add_subdirectory(src)

#set(CMAKE_CXX_CLANG_TIDY clang-tidy-19 -config-file=${CMAKE_SOURCE_DIR}/.clang-tidy)
