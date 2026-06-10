add_subdirectory(datatypes)

qt_add_executable(CutieDesigner
  main.cpp
)

qt_add_qml_module(CutieDesigner
  URI CutieDesigner
  QML_FILES
  Main.qml

  nodes/input/ColorInput/ColorInputControl.qml
  nodes/input/NumberInput/NumberInputControl.qml
  nodes/display/AType/Ukrug/Ukrug.qml
  nodes/display/AType/Ukrug/UkrugShader.qml
  nodes/display/AType/Ukrug/UkrugControl.qml
  nodes/display/AType/Ukrug/UkrugCharacter.qml
  nodes/display/AType/AType.qml
  nodes/display/fill/FillSurface.qml
  nodes/display/shaderEffects/Blend/Blend.qml
  nodes/display/shaderEffects/Blend/BlendControl.qml
  nodes/display/video/VideoDisplay.qml
  nodes/display/Stack/Stack.qml
  nodes/display/Stack/StackControl.qml
  nodes/process/TextTyper/TextTyper.qml

  SOURCES
  nodes/input/ColorInput/ColorInputNode.hpp
  nodes/input/ColorInput/ColorInputNode.cpp
  nodes/input/NumberInput/NumberInputNode.hpp
  nodes/input/NumberInput/NumberInputNode.cpp
  nodes/display/AType/Ukrug/UkrugNode.hpp
  nodes/display/AType/Ukrug/UkrugNode.cpp
  nodes/display/AType/Ukrug/UkrugPointsList.hpp
  nodes/display/AType/Ukrug/UkrugPointsList.cpp
  nodes/display/AType/ATypeCharacterNodeModel.hpp
  nodes/display/AType/ATypeCharacterNodeModel.cpp
  nodes/display/AType/ATypeNode.hpp
  nodes/display/AType/ATypeNode.cpp
  nodes/display/fill/FillNode.hpp
  nodes/display/fill/FillNode.cpp
  nodes/display/Stack/StackNode.cpp
  nodes/display/Stack/StackNode.cpp
  nodes/display/surface/SurfaceDisplayNode.hpp
  nodes/display/surface/SurfaceDisplayNode.cpp
  nodes/display/surface/SurfaceLoader.hpp
  nodes/display/surface/SurfaceLoader.cpp
  nodes/display/shaderEffects/Blend/BlendNode.cpp
  nodes/display/shaderEffects/Blend/BlendNode.hpp
  nodes/display/video/VideoDisplayNode.hpp
  nodes/display/video/VideoDisplayNode.cpp
  nodes/process/math/SubtractionNode.hpp
  nodes/process/math/AdditionNode.hpp
  nodes/process/math/DivisionNode.hpp
  nodes/process/math/MathOperationNodeModel.hpp
  nodes/process/math/MathOperationNodeModel.cpp
  nodes/process/math/MultiplicationNode.hpp
  nodes/process/math/SinNode.hpp
  nodes/process/math/CosNode.hpp
  nodes/process/TextTyper/TextTyperNode.hpp
  nodes/process/TextTyper/TextTyperNode.cpp
  nodes/process/TextTyper/TextTyperEventList.hpp
  nodes/process/TextTyper/TextTyperEventList.cpp

  DEPENDENCIES QtQuick QtQuick.Layout StatsModule NodeModule CutieUiModule
  RESOURCES
  nodes/display/AType/Ukrug/UkrugPointsData.json
  nodes/display/AType/Ukrug/ukrug.vert.qsb
  nodes/display/AType/Ukrug/ukrug.frag.qsb
)


qt6_add_shaders(CutieDesigner "CutieDesigner_shaders"
  PREFIX
    "/qt/qml/CutieDesigner"
  FILES
  nodes/display/shaderEffects/Blend/blend.vert
  nodes/display/shaderEffects/Blend/blend.frag
)
target_link_directories(CutieDesigner
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${LIB_TARGET_LINK_DIRECTORIES}
  ${GSTREAMER_LIBRARY_DIRS}
)

target_include_directories(CutieDesigner
  PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${GSTREAMER_INCLUDE_DIRS}
  nodes
  nodes/display
  nodes/display/AType
  nodes/display/AType/Ukrug
  nodes/display/surface
  nodes/display/fill
  nodes/display/Stack
  nodes/display/shaderEffects
  nodes/display/shaderEffects/Blend
  nodes/display/video
  nodes/input
  nodes/input/ColorInput/
  nodes/input/NumberInput/
  nodes/process
  nodes/process/math
  nodes/process/TextTyper
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(CutieDesigner PROPERTIES
  #    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.CutieDesigner
  MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
  MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
  MACOSX_BUNDLE TRUE
  WIN32_EXECUTABLE TRUE
)

target_compile_definitions(CutieDesigner
  PRIVATE $<$<CONFIG:Debug>:QT_QML_DEBUG>)

target_link_libraries(CutieDesigner PRIVATE
  Qt6::Core
  Qt6::Gui
  Qt6::Quick
  Qt6::QuickWidgets
  Qt6::Qml
  StatsModule
  NodeModule
  CutieUiModule
  vivid
  datatypes
  ${GSTREAMER_LIBRARIES}
)
target_compile_definitions(CutieDesigner PRIVATE GLM_ENABLE_EXPERIMENTAL)

install(TARGETS CutieDesigner
  BUNDLE DESTINATION .
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
