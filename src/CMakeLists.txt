add_subdirectory(graph)

qt_add_executable(CutieDesigner
    main.cpp
)

qt_add_qml_module(CutieDesigner
    URI Sandbox
    QML_FILES
    Main.qml
    DEPENDENCIES QtQuick
)



target_link_directories(CutieDesigner
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/graph
    ${LIB_TARGET_LINK_DIRECTORIES}
)

target_include_directories(CutieDesigner
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/
    ${CMAKE_CURRENT_SOURCE_DIR}/graph
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
    PRIVATE Qt6::Quick QtNodes::QtNodes graph
)

install(TARGETS CutieDesigner
    BUNDLE DESTINATION ${PROJECT_SOURCE_DIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
