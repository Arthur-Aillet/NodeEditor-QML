qt_add_qml_module(display
  URI CutieDesigner.Nodes.Display
  SHARED
  QML_FILES
  AType/Ukrug/Ukrug.qml
  AType/Ukrug/UkrugShader.qml
  AType/Ukrug/UkrugControl.qml
  AType/Ukrug/UkrugCharacter.qml
  AType/AType.qml
  Dimension/Dimension.qml
  fill/FillSurface.qml
  shaderEffects/Blend/Blend.qml
  shaderEffects/Blend/BlendControl.qml
  video/VideoDisplay.qml
  Stack/Stack.qml
  Stack/StackControl.qml

  SOURCES
  AType/Ukrug/UkrugNode.hpp
  AType/Ukrug/UkrugNode.cpp
  AType/Ukrug/UkrugPointsList.hpp
  AType/Ukrug/UkrugPointsList.cpp
  AType/ATypeCharacterNodeModel.hpp
  AType/ATypeCharacterNodeModel.cpp
  AType/ATypeNode.hpp
  AType/ATypeNode.cpp
  Dimension/DimensionNode.hpp
  Dimension/DimensionNode.cpp
  fill/FillNode.hpp
  fill/FillNode.cpp
  Stack/StackNode.cpp
  Stack/StackNode.cpp
  surface/SurfaceDisplayNode.hpp
  surface/SurfaceDisplayNode.cpp
  surface/SurfaceLoader.hpp
  surface/SurfaceLoader.cpp
  shaderEffects/Blend/BlendNode.cpp
  shaderEffects/Blend/BlendNode.hpp
  video/VideoDisplayNode.hpp
  video/VideoDisplayNode.cpp

  DEPENDENCIES QtQuick QtQuick.Layout NodeEditor CutieUiModule CutieDesigner.Datatypes
  RESOURCES
  AType/Ukrug/UkrugPointsData.json
  AType/Ukrug/ukrug.vert.qsb
  AType/Ukrug/ukrug.frag.qsb
)


qt6_add_shaders(display "display_shaders"
  PREFIX
    "/qt/qml/CutieDesigner/Nodes/Display"
  FILES
  shaderEffects/Blend/blend.vert
  shaderEffects/Blend/blend.frag
)

target_link_directories(display
  PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${LIB_TARGET_LINK_DIRECTORIES}
  ${GSTREAMER_LIBRARY_DIRS}
)
  
target_include_directories(display
  PUBLIC
  ${GSTREAMER_INCLUDE_DIRS}
  Dimension
  AType
  AType/Ukrug
  surface
  fill
  Stack
  shaderEffects
  shaderEffects/Blend
  video
)

target_link_libraries(display PUBLIC
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

