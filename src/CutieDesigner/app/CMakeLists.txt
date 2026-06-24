qt_add_qml_module(app
  STATIC
  URI CutieDesigner.App
  QML_FILES
  AppLayout.qml
  Timeline.qml
  NodeView.qml

  DEPENDENCIES QtQuick QtQuick.Layout NodeEditor CutieDesigner.Nodes.Display CutieDesigner.Stats CutieUiModule
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
  Qt6::QuickWidgets
  Qt6::Qml
  NodeEditor
  CutieUiModule
  nodes
)
