cmake_minimum_required(VERSION 3.10..4.0)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)

set(FETCHCONTENT_BASE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(dependencies)

project(CutieDesigner VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
#set(CMAKE_PREFIX_PATH "~/Qt/6.11.0/gcc_64/")
#set(LD_LIBRARY_PATH "~/Qt/6.11.0/gcc_64/lib")
set(QT_QML_GENERATE_QMLLS_INI ON)

find_package(Qt6 REQUIRED COMPONENTS Quick Core Gui QuickWidgets QuickControls2 Qml Core5Compat ShaderTools)

cmake_policy(SET CMP0071 NEW)

qt_standard_project_setup()
qt_policy(SET QTP0001 NEW)
qt_policy(SET QTP0004 NEW)

#set(CMAKE_CXX_CLANG_TIDY clang-tidy-19 -config-file=${CMAKE_SOURCE_DIR}/.clang-tidy)

option(DEBUG_MODE "Set DEBUG Mode" ON)
option(RUN "Launch App" OFF)


set(QML_IMPORT_PATH
  ${CMAKE_BINARY_DIR}/qml
  CACHE STRING "" FORCE
)
set(QT_QML_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/qml)

add_subdirectory(src)

if(RUN)
  add_custom_target(run
    COMMENT "Launching binary"
    COMMAND CutieDesigner
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  )
endif()
