qt_add_qml_module(NodeModule
  URI NodeModule
  QML_FILES
  ./GraphicsView/GraphicsView.qml
  DEPENDENCIES QtQuick
  SOURCES
  NodeEditor.hpp
  NodeEditor.cpp
  NodeModel.cpp
  NodeModel.hpp
  NodeData.hpp
  ./nodes/ValueNodeModel.hpp
  ./nodes/ValueNodeModel.cpp
  ./GraphicsView/ViewBackground.cpp
  ./GraphicsView/ViewBackground.hpp
)

target_include_directories(NodeModule PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
  ./nodes
  ./GraphicsView
)

target_link_libraries(NodeModule
  PUBLIC Qt6::Quick QtNodes::QtNodes
)
