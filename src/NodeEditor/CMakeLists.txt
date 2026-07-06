qt_add_qml_module(NodeEditor
  SHARED
  URI NodeEditor
  QML_FILES
  nodes/DefaultNodePainter.qml
  nodes/NodeGraphicalObject.qml
  nodes/AbstractNodePainter.qml
  nodes/DefaultPortPainter.qml
  nodes/NodeList.qml
  nodes/QSet.qml

  view/GraphicsView.qml
  view/NavigableArea.qml
  view/ViewBackground.qml

  connections/DefaultConnection.qml
  connections/DraftConnection.qml
  connections/ConnectionList.qml

  DEPENDENCIES QtQuick

  SOURCES
  interface/ModelInterface.cpp
  interface/ModelInterface.hpp
  interface/DataFlowModelInterface.cpp
  interface/DataFlowModelInterface.hpp
  interface/QmlUndoCommands.cpp
  interface/QmlUndoCommands.hpp

  style/ConnectionStyle.cpp
  style/ConnectionStyle.hpp
  style/NodeStyle.cpp
  style/NodeStyle.hpp
  style/Style.hpp
  style/StyleCollection.cpp
  style/StyleCollection.hpp
  style/GraphicsViewStyle.cpp
  style/GraphicsViewStyle.hpp

  data/AbstractGraphModel.cpp
  data/AbstractGraphModel.hpp
  data/ConnectionIdHash.hpp
  data/ConnectionIdUtils.hpp
  data/DataFlowGraphModel.cpp
  data/DataFlowGraphModel.hpp
  data/Definitions.hpp
  data/NodeData.cpp
  data/NodeData.hpp
  data/NodeDelegateModel.cpp
  data/NodeDelegateModel.hpp
  data/NodeDelegateModelRegistry.cpp
  data/NodeDelegateModelRegistry.hpp
  data/QStringStdHash.hpp
  data/Serializable.hpp
  data/RegisteredNodeModel.cpp
  data/RegisteredNodeModel.hpp

  RESOURCES
  resources/resources.qrc
)


qt6_add_shaders(NodeEditor "NodeEditor_shaders"
  PREFIX
  "/qt/qml/NodeEditor"
  FILES
  view/viewBackground.frag
  view/viewBackground.vert
)

target_include_directories(NodeEditor PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
  nodes
  view
  interface
  style
  data
)

target_link_libraries(NodeEditor
  PUBLIC Qt6::Quick Qt6::Core vivid
)

target_compile_definitions(NodeEditor PRIVATE GLM_ENABLE_EXPERIMENTAL)

