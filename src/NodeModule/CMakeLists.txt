qt_add_qml_module(NodeModule
  URI NodeModule
  QML_FILES
  nodes/DefaultNodePainter.qml
  nodes/NodeGraphicalObject.qml
  nodes/AbstractNodePainter.qml
  nodes/DefaultPortPainter.qml
  nodes/NodeList.qml
  nodes/QSet.qml

  view/GraphicsView.qml
  view/NavigableArea.qml
  view/SceneMenu.qml

  connections/DefaultConnection.qml
  connections/DraftConnection.qml
  connections/ConnectionList.qml

  DEPENDENCIES QtQuick

  SOURCES
  view/ViewBackground.cpp
  view/ViewBackground.hpp

  interface/ModelInterface.cpp
  interface/ModelInterface.hpp
  interface/DataFlowModelInterface.cpp
  interface/DataFlowModelInterface.hpp
  interface/RegisteryAccess.cpp
  interface/RegisteryAccess.hpp
  interface/QmlUndoCommands.cpp
  interface/QmlUndoCommands.hpp
  interface/NodeGeometryInterface.cpp
  interface/NodeGeometryInterface.hpp

  style/QmlNodeStyle.hpp
  style/QmlConnectionStyle.hpp
  style/QmlGraphicsViewStyle.hpp
  style/StyleCollection.hpp

  nodes/QmlDefinition.hpp
)

target_include_directories(NodeModule PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
  nodes
  view
  interface
  style
)

target_link_libraries(NodeModule
  PUBLIC Qt6::Quick QtNodes::QtNodes
)
