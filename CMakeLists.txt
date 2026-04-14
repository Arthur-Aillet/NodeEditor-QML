cmake_minimum_required(VERSION 3.16)

project(appSandbox VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_PREFIX_PATH "~/Qt/6.11.0/gcc_64/")
set(QML_IMPORT_PATH "~/Qt/6.11.0/gcc_64/qml/")
set(QT_QML_GENERATE_QMLLS_INI ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_CLANG_TIDY clang-tidy -header-filter=. -config-file=../.clang-tidy)

option(RUN "Launch App" OFF)
if(RUN)
    add_custom_target(run
        COMMENT "Launching binary"
        COMMAND ./build/appSandbox
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
endif()

option(DEBUG_MODE "Set DEBUG Mode" ON)

find_package(Qt6 REQUIRED COMPONENTS Quick Core)

qt_standard_project_setup(REQUIRES 6.10)

qt_add_executable(appSandbox
    src/main.cpp
    src/Receiver.hpp
    src/Receiver.cpp
    src/observers/IncrementalObserver.cpp
    src/observers/IncrementalObserver.hpp
    src/receivers/ColorSceneReceiver.hpp
    src/Orchestrator.cpp
    src/Orchestrator.hpp
    src/Observer.hpp
)

qt_add_qml_module(appSandbox
    URI Sandbox
    QML_FILES
    src/Main.qml
    src/scenes/ColorScene.qml
    DEPENDENCIES QtQuick
)

target_link_directories(appSandbox
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/receivers
    ${CMAKE_CURRENT_SOURCE_DIR}/src/observers
    ${CMAKE_CURRENT_SOURCE_DIR}/src/scenes
    ${LIB_TARGET_LINK_DIRECTORIES}
)

target_include_directories(appSandbox
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/observers
    ${CMAKE_CURRENT_SOURCE_DIR}/src/receivers
    ${CMAKE_CURRENT_SOURCE_DIR}/src/scenes
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appSandbox PROPERTIES
    #    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appSandbox
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appSandbox
    PRIVATE Qt6::Quick
)

install(TARGETS appSandbox
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
