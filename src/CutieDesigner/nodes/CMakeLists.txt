qt_add_qml_module(nodes
  URI CutieDesigner.Nodes
  QML_FILES

  input/ColorInput/ColorInputControl.qml
  input/NumberInput/NumberInputControl.qml
  display/AType/Ukrug/Ukrug.qml
  display/AType/Ukrug/UkrugShader.qml
  display/AType/Ukrug/UkrugControl.qml
  display/AType/Ukrug/UkrugCharacter.qml
  display/AType/AType.qml
  display/fill/FillSurface.qml
  display/shaderEffects/Blend/Blend.qml
  display/shaderEffects/Blend/BlendControl.qml
  display/video/VideoDisplay.qml
  display/Stack/Stack.qml
  display/Stack/StackControl.qml
  process/TextTyper/TextTyper.qml

  SOURCES
  input/ColorInput/ColorInputNode.hpp
  input/ColorInput/ColorInputNode.cpp
  input/NumberInput/NumberInputNode.hpp
  input/NumberInput/NumberInputNode.cpp
  display/AType/Ukrug/UkrugNode.hpp
  display/AType/Ukrug/UkrugNode.cpp
  display/AType/Ukrug/UkrugPointsList.hpp
  display/AType/Ukrug/UkrugPointsList.cpp
  display/AType/ATypeCharacterNodeModel.hpp
  display/AType/ATypeCharacterNodeModel.cpp
  display/AType/ATypeNode.hpp
  display/AType/ATypeNode.cpp
  display/fill/FillNode.hpp
  display/fill/FillNode.cpp
  display/Stack/StackNode.cpp
  display/Stack/StackNode.cpp
  display/surface/SurfaceDisplayNode.hpp
  display/surface/SurfaceDisplayNode.cpp
  display/surface/SurfaceLoader.hpp
  display/surface/SurfaceLoader.cpp
  display/shaderEffects/Blend/BlendNode.cpp
  display/shaderEffects/Blend/BlendNode.hpp
  display/video/VideoDisplayNode.hpp
  display/video/VideoDisplayNode.cpp
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

  DEPENDENCIES QtQuick QtQuick.Layout NodeEditor CutieUiModule CutieDesigner.Datatypes
  RESOURCES
  display/AType/Ukrug/UkrugPointsData.json
  display/AType/Ukrug/ukrug.vert.qsb
  display/AType/Ukrug/ukrug.frag.qsb
)


qt6_add_shaders(nodes "nodes_shaders"
  PREFIX
    "/qt/qml/CutieDesigner/Nodes"
  FILES
  display/shaderEffects/Blend/blend.vert
  display/shaderEffects/Blend/blend.frag
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
  display
  display/AType
  display/AType/Ukrug
  display/surface
  display/fill
  display/Stack
  display/shaderEffects
  display/shaderEffects/Blend
  display/video
  input
  input/ColorInput/
  input/NumberInput/
  process
  process/math
  process/TextTyper
)

target_link_libraries(nodes PRIVATE
  Qt6::Core
  Qt6::Gui
  Qt6::Quick
  Qt6::QuickWidgets
  Qt6::Qml
  NodeEditor
  CutieUiModule
  vivid
  datatypes
  ${GSTREAMER_LIBRARIES}
)

target_compile_definitions(nodes PRIVATE GLM_ENABLE_EXPERIMENTAL)
