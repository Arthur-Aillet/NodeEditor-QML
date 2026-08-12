qt_add_qml_module(NodeEditor
  SHARED
  URI NodeEditor
  QML_FILES
  connections/DefaultConnection.qml
  connections/DraftConnection.qml
  connections/ConnectionList.qml

  controls/DefaultControlsView.qml
  controls/NodeListMenu.qml
  controls/NodeSearchMenu.qml
  controls/NodeSearchTextField.qml
  controls/PortSearchMenu.qml
  controls/SearchFilterModel.qml
  controls/SearchMenuItem.qml

  nodes/DefaultNodePainter.qml
  nodes/NodeGraphicalObject.qml
  nodes/AbstractNodePainter.qml
  nodes/DefaultPortPainter.qml
  nodes/NodeList.qml
  nodes/QSet.qml

  view/GraphicsView.qml
  view/NavigableArea.qml
  view/ViewBackground.qml


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

  nodes/Clipboard.hpp
  nodes/Clipboard.cpp

  RESOURCES
  controls/icomoon.ttf
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