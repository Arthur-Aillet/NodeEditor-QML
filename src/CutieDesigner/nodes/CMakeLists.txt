add_subdirectory(display)
add_subdirectory(input)

qt_add_qml_module(nodes
  URI CutieDesigner.Nodes
  SHARED
  QML_FILES

  process/TextTyper/TextTyper.qml

  SOURCES
  process/color/CombineRGBA.cpp
  process/color/CombineRGBA.hpp
  process/color/SplitRGBA.cpp
  process/color/SplitRGBA.hpp
  process/math/SubtractionNode.hpp
  process/math/AdditionNode.hpp
  process/math/DivisionNode.hpp
  process/math/MathOperationNodeModel.hpp
  process/math/MathOperationNodeModel.cpp
  process/math/MultiplicationNode.hpp
  process/math/SinNode.hpp
  process/math/CosNode.hpp
  process/TextTyper/TextTyperNode.hpp
  process/TextTyper/TextTyperNode.cpp
  process/TextTyper/TextTyperEventList.hpp
  process/TextTyper/TextTyperEventList.cpp
  process/vectors/CombineVec2.cpp
  process/vectors/CombineVec2.hpp
  process/vectors/SplitVec2.cpp
  process/vectors/SplitVec2.hpp

  DEPENDENCIES QtQuick QtQuick.Layout NodeEditor CutieUiModule CutieDesigner.Datatypes
)

target_link_directories(nodes
  PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${LIB_TARGET_LINK_DIRECTORIES}
  ${GSTREAMER_LIBRARY_DIRS}
  )
  
  target_include_directories(nodes
  PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${GSTREAMER_INCLUDE_DIRS}

  process/color
  process/math
  process/TextTyper
  process/vectors
)

target_link_libraries(nodes PUBLIC
  Qt6::Core
  Qt6::Gui
  Qt6::Quick
  Qt6::QuickWidgets
  Qt6::Qml
  NodeEditor
  CutieUiModule
  vivid
  datatypes
  display
  input
  ${GSTREAMER_LIBRARIES}
)

target_compile_definitions(nodes PRIVATE GLM_ENABLE_EXPERIMENTAL)
