cmake_minimum_required(VERSION 3.16)

set(PROJECT_NAME "appSandbox" CACHE STRING "Project Name.")

project(${PROJECT_NAME} VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_PREFIX_PATH "~/Qt/6.11.0/gcc_64/")
set(QML_IMPORT_PATH "~/Qt/6.11.0/gcc_64/qml/")
set(QT_QML_GENERATE_QMLLS_INI ON)

option(RUN "Launch App" OFF)
if(RUN)
    add_custom_target(run
        COMMENT "Launching binary"
        COMMAND ./build/${PROJECT_NAME}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
endif()

option(DEBUG_MODE "Set DEBUG Mode" ON)

find_package(Qt6 REQUIRED COMPONENTS Quick Core)

qt_standard_project_setup(REQUIRES 6.10)

qt_add_executable(${PROJECT_NAME}
    src/main.cpp
)

qt_add_qml_module(${PROJECT_NAME}
    URI Sandbox
    QML_FILES
        src/Main.qml
)

target_link_directories(${PROJECT_NAME}
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${LIB_TARGET_LINK_DIRECTORIES}
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(${PROJECT_NAME} PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.${PROJECT_NAME}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE Qt6::Quick
)

install(TARGETS ${PROJECT_NAME}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
