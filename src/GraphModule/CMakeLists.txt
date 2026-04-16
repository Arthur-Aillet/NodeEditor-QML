qt_add_qml_module(GraphModule
  URI GraphModule
  QML_FILES
  GraphView.qml
  DEPENDENCIES QtQuick
  SOURCES
  GraphModel.cpp
  GraphModel.hpp
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

target_include_directories(GraphModule PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
  ./nodes
  ./GraphicsView
)

target_link_libraries(GraphModule
  PUBLIC Qt6::Quick QtNodes::QtNodes
)
