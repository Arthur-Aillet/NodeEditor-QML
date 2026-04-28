qt_add_qml_module(NodeModule
  URI NodeModule
  QML_FILES
  ./Node/DefaultNode.qml
  ./Node/NodeObject.qml
  ./GraphicsView/GraphicsView.qml
  ./GraphicsView/NavigableArea.qml
  ./GraphicsView/SceneMenu.qml
  ./Connection/DefaultConnection.qml
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
  ./ModelInterface/ModelInterface.cpp
  ./ModelInterface/ModelInterface.hpp
  ./ModelInterface/DataFlowModelInterface.cpp
  ./ModelInterface/DataFlowModelInterface.hpp
  ./ModelInterface/RegisteryAccess.cpp
  ./ModelInterface/RegisteryAccess.hpp
  ./ModelInterface/QmlUndoCommands.cpp
  ./ModelInterface/QmlUndoCommands.hpp
  ./ModelInterface/NodeGeometryInterface.cpp
  ./ModelInterface/NodeGeometryInterface.hpp
  ./Style/QmlNodeStyle.hpp
  ./Node/QmlDefinition.hpp
)

target_include_directories(NodeModule PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
  ./nodes
  ./GraphicsView
  ./ModelInterface
  ./Style
  ./Node
)

target_link_libraries(NodeModule
  PUBLIC Qt6::Quick QtNodes::QtNodes
)
