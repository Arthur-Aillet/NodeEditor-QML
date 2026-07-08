qt_add_qml_module(menu
  STATIC
  URI CutieDesigner.App.Menu
  QML_FILES
  NodeListMenu.qml
  NodeSearchMenu.qml
  PortSearchMenu.qml
  SearchMenuItem.qml
  NodeSearchTextField.qml
  SearchFilterModel.qml

  DEPENDENCIES QtQuick QtQuick.Controls QtQuick.Layout NodeEditor 
  RESOURCES
  icomoon.ttf
)

target_link_directories(menu
  PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${LIB_TARGET_LINK_DIRECTORIES}
)

target_include_directories(menu PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(menu PRIVATE
  Qt6::Core
  Qt6::Quick
  Qt6::Qml
  NodeEditor
)
