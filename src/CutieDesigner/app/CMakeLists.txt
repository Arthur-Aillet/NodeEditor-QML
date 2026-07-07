add_subdirectory(menu)
add_subdirectory(FileManager)

qt_add_qml_module(app
  STATIC
  URI CutieDesigner.App
  QML_FILES
  AppLayout.qml
  Timeline.qml
  TopBar.qml
  NodeView.qml

  DEPENDENCIES 
  QtQuick QtQuick.Control QtQuick.Layout QtQuick.Dialogs
  CutieDesigner.Nodes.Display CutieDesigner.Stats CutieDesigner.App.Menu CutieDesigner.App.FileManager
  CutieUiModule
  NodeEditor 
)

target_link_directories(app
  PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${LIB_TARGET_LINK_DIRECTORIES}
)

target_include_directories(app PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(app PRIVATE
  Qt6::Core
  Qt6::Gui
  Qt6::Quick
  Qt6::Qml
  NodeEditor
  CutieUiModule
  menu
  display
  time
)

target_link_libraries(app PUBLIC
  fileManager
)
