add_subdirectory(datatypes)
add_subdirectory(time)
add_subdirectory(nodes)
add_subdirectory(stats)
add_subdirectory(app)

qt_add_executable(CutieDesigner
  main.cpp
)

qt_add_qml_module(CutieDesigner
  URI CutieDesigner
  QML_FILES
  Main.qml

  DEPENDENCIES QtQuick CutieDesigner.App
)

target_link_directories(CutieDesigner
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${LIB_TARGET_LINK_DIRECTORIES}
  ${GSTREAMER_LIBRARY_DIRS}
)

target_include_directories(CutieDesigner PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

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

target_compile_definitions(CutieDesigner
  PRIVATE $<$<CONFIG:Debug>:QT_QML_DEBUG> GLM_ENABLE_EXPERIMENTAL)

target_link_libraries(CutieDesigner PRIVATE
  Qt6::Core
  Qt6::Gui
  Qt6::Quick
  Qt6::QuickWidgets
  Qt6::Qml
  NodeEditor
  datatypes
  stats
  nodes
  ${GSTREAMER_LIBRARIES}
)

install(TARGETS CutieDesigner
  BUNDLE DESTINATION .
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
