cmake_minimum_required(VERSION 3.16)

project(CutieDesigner VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_PREFIX_PATH "~/Qt/6.11.0/gcc_64/")
set(LD_LIBRARY_PATH "~/Qt/6.11.0/gcc_64/lib")
set(QT_QML_GENERATE_QMLLS_INI ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Qt6 REQUIRED COMPONENTS Quick Core Gui QuickWidgets QuickControls2 Qml Core5Compat)

qt_standard_project_setup()
qt_policy(SET QTP0001 NEW)
qt_policy(SET QTP0004 NEW)

#set(CMAKE_CXX_CLANG_TIDY clang-tidy-19 -config-file=${CMAKE_SOURCE_DIR}/.clang-tidy)

option(DEBUG_MODE "Set DEBUG Mode" ON)
option(RUN "Launch App" OFF)

#add_subdirectory(external/nodeeditor)

add_subdirectory(src)

if(RUN)
  add_custom_target(run
    COMMENT "Launching binary"
    COMMAND CutieDesigner
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  )
endif()
