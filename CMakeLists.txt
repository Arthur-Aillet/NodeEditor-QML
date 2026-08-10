cmake_minimum_required(VERSION 3.10..4.0)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

cmake_policy(SET CMP0071 NEW)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Quick Qml ShaderTools)

qt_policy(SET QTP0001 NEW)
qt_policy(SET QTP0004 NEW)

add_subdirectory(src)

#set(CMAKE_CXX_CLANG_TIDY clang-tidy-19 -config-file=${CMAKE_SOURCE_DIR}/.clang-tidy)
