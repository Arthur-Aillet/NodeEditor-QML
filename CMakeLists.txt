cmake_minimum_required(VERSION 3.16)

project(Sandbox VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_PREFIX_PATH "~/Qt/6.11.0/gcc_64/")
set(QML_IMPORT_PATH "~/Qt/6.11.0/gcc_64/qml/")

find_package(Qt6 REQUIRED COMPONENTS Quick)

qt_standard_project_setup(REQUIRES 6.10)

qt_add_executable(appSandbox
    src/main.cpp
)

qt_add_qml_module(appSandbox
    URI Sandbox
    QML_FILES
        src/Main.qml
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
