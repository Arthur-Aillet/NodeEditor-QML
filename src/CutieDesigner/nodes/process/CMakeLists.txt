qt_add_qml_module(process
  URI CutieDesigner.Nodes.Process
  SHARED
  QML_FILES

  TextTyper/TextTyperControl.qml
  color/ColorModeControl.qml

  SOURCES
  color/CombineColorNode.cpp
  color/CombineColorNode.hpp
  color/ColorModes.cpp
  color/ColorModes.hpp
  color/SplitColorNode.cpp
  color/SplitColorNode.hpp
  math/SubtractionNode.hpp
  math/AdditionNode.hpp
  math/DivisionNode.hpp
  math/MathOperationNodeModel.hpp
  math/MathOperationNodeModel.cpp
  math/MultiplicationNode.hpp
  math/SinNode.hpp
  math/CosNode.hpp
  TextTyper/TextTypeEvent.hpp
  TextTyper/TextTypeEvent.cpp
  TextTyper/TextTyperNode.hpp
  TextTyper/TextTyperNode.cpp
  TextTyper/TextTyperEventList.hpp
  TextTyper/TextTyperEventList.cpp
  vectors/CombineVec2Node.cpp
  vectors/CombineVec2Node.hpp
  vectors/SplitVec2Node.cpp
  vectors/SplitVec2Node.hpp

  DEPENDENCIES QtQuick QtQuick.Layout NodeEditor CutieUiModule CutieDesigner.Datatypes
)

target_link_directories(process
  PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${LIB_TARGET_LINK_DIRECTORIES}
)

target_include_directories(process
  PUBLIC
  color
  math
  TextTyper
  vectors
)

target_link_libraries(process PUBLIC
  Qt6::Core
  Qt6::Gui
  Qt6::Quick
  Qt6::QuickWidgets
  Qt6::Qml
  NodeEditor
  CutieUiModule
  vivid
  datatypes
)

target_compile_definitions(process PRIVATE GLM_ENABLE_EXPERIMENTAL)
