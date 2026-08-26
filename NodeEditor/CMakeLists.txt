qt_add_qml_module(NodeEditor
  SHARED
  URI NodeEditor
  QML_FILES
  connections/AbstractConnectionPainter.qml
  connections/ConnectionList.qml
  connections/ConnectionObject.qml
  connections/DefaultConnectionPainter.qml
  connections/DraftConnection.qml

  controls/DefaultControlsView.qml
  controls/NodeListMenu.qml
  controls/NodeSearchMenu.qml
  controls/NodeSearchTextField.qml
  controls/PortSearchMenu.qml
  controls/SearchFilterModel.qml
  controls/SearchMenuItem.qml

  nodes/AbstractNodePainter.qml
  nodes/DefaultNodePainter.qml
  nodes/DefaultPortPainter.qml
  nodes/NodeList.qml
  nodes/NodeObject.qml

  view/GraphicsView.qml
  view/NavigableArea.qml
  view/ViewBackground.qml

  DEPENDENCIES QtQuick

  SOURCES
  data/AbstractContext.cpp
  data/AbstractContext.hpp
  data/AbstractGraph.cpp
  data/AbstractGraph.hpp
  data/ConnectionIdHash.hpp
  data/ConnectionIdUtils.hpp
  data/DataFlowContext.hpp
  data/DataFlowGraph.cpp
  data/DataFlowGraph.hpp
  data/Definitions.hpp
  data/NodeData.cpp
  data/NodeData.hpp
  data/NodeModel.cpp
  data/NodeModel.hpp
  data/NodeModelRegistry.cpp
  data/NodeModelRegistry.hpp
  data/QStringStdHash.hpp
  data/RegisteredNodeModel.cpp
  data/RegisteredNodeModel.hpp
  data/Serializable.hpp

  nodes/Clipboard.cpp
  nodes/Clipboard.hpp

  style/ConnectionStyle.cpp
  style/ConnectionStyle.hpp
  style/GraphicsViewStyle.cpp
  style/GraphicsViewStyle.hpp
  style/NodeStyle.cpp
  style/NodeStyle.hpp
  style/Style.hpp
  style/StyleCollection.cpp
  style/StyleCollection.hpp

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
  PRIVATE Qt::Core Qt::Gui Qt::Qml Qt::Quick
)

install(TARGETS NodeEditor
  BUNDLE  DESTINATION .
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
