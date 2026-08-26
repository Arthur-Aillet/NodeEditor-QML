cmake_minimum_required(VERSION 3.10..4.0)

project(NodeEditor CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(QT_QML_GENERATE_QMLLS_INI ON)

option(BUILD_EXAMPLES "Build Examples" "${QT_NODES_DEVELOPER_DEFAULTS}")

cmake_policy(SET CMP0071 NEW)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Quick Qml ShaderTools)

qt_policy(SET QTP0001 NEW)
qt_policy(SET QTP0004 NEW)

add_subdirectory(NodeEditor)

if(BUILD_EXAMPLES)
  add_subdirectory(examples)
endif()

#set(CMAKE_CXX_CLANG_TIDY clang-tidy-19 -config-file=${CMAKE_SOURCE_DIR}/.clang-tidy)
