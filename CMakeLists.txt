cmake_minimum_required(VERSION 3.16)

project(CutieDesigner VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_PREFIX_PATH "~/Qt/6.11.0/gcc_64/")
set(QML_IMPORT_PATH "~/Qt/6.11.0/gcc_64/qml/")
set(QT_QML_GENERATE_QMLLS_INI ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_CLANG_TIDY clang-tidy -config-file=../.clang-tidy)

option(RUN "Launch App" OFF)
if(RUN)
    add_custom_target(run
        COMMENT "Launching binary"
        COMMAND ./build/CutieDesigner
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
endif()

option(DEBUG_MODE "Set DEBUG Mode" ON)

find_package(Qt6 REQUIRED COMPONENTS Quick Core)

qt_standard_project_setup(REQUIRES 6.10)

add_subdirectory(external/nodeeditor)

qt_add_executable(CutieDesigner
    src/main.cpp
    src/graph/GraphModel.cpp
    src/graph/GraphModel.hpp
)

qt_add_qml_module(CutieDesigner
    URI Sandbox
    QML_FILES
    src/Main.qml
    DEPENDENCIES QtQuick
)

target_link_directories(CutieDesigner
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/graph
    ${LIB_TARGET_LINK_DIRECTORIES}
)

target_include_directories(CutieDesigner
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/graph
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(CutieDesigner PROPERTIES
    #    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.CutieDesigner
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(CutieDesigner
    PRIVATE Qt6::Quick QtNodes::QtNodes
)

install(TARGETS CutieDesigner
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
