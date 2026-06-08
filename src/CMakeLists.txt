add_subdirectory(NodeModule)
add_subdirectory(CutieUiModule)
add_subdirectory(StatsModule)

qt_add_executable(CutieDesigner
  main.cpp
)

qt_add_qml_module(CutieDesigner
  URI CutieDesignerModule
  QML_FILES
  Main.qml

  nodes/input/PortLabel.qml
  nodes/display/AType/Ukrug/Ukrug.qml
  nodes/display/AType/Ukrug/UkrugShader.qml
  nodes/display/AType/Ukrug/UkrugControl.qml
  nodes/display/AType/Ukrug/UkrugCharacter.qml
  nodes/display/AType/AType.qml
  nodes/display/shaderEffects/Blend.qml
  nodes/display/shaderEffects/BlendControl.qml
  nodes/process/TextTyper/TextTyper.qml

  SOURCES
  datatypes/ATypeCharacterData.hpp
  datatypes/SurfaceData.hpp
  datatypes/SurfaceData.cpp
  datatypes/DecimalData.hpp
  datatypes/TextData.hpp
  datatypes/TextData.cpp

  nodes/input/NumberInputNode.hpp
  nodes/input/NumberInputNode.cpp
  nodes/display/AType/Ukrug/UkrugNode.hpp
  nodes/display/AType/Ukrug/UkrugNode.cpp
  nodes/display/AType/Ukrug/UkrugPointsList.hpp
  nodes/display/AType/Ukrug/UkrugPointsList.cpp
  nodes/display/AType/ATypeCharacterNodeModel.hpp
  nodes/display/AType/ATypeCharacterNodeModel.cpp
  nodes/display/AType/ATypeNode.hpp
  nodes/display/AType/ATypeNode.cpp
  nodes/display/surface/SurfaceDisplayNode.hpp
  nodes/display/surface/SurfaceDisplayNode.cpp
  nodes/display/surface/SurfaceLoader.hpp
  nodes/display/surface/SurfaceLoader.cpp
  nodes/display/shaderEffects/BlendNode.cpp
  nodes/display/shaderEffects/BlendNode.hpp
  nodes/process/math/SubtractionNode.hpp
  nodes/process/math/AdditionNode.hpp
  nodes/process/math/DivisionNode.hpp
  nodes/process/math/MathOperationNodeModel.hpp
  nodes/process/math/MathOperationNodeModel.cpp
  nodes/process/math/MultiplicationNode.hpp
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
    "/qt/qml/CutieDesignerModule"
  FILES
  nodes/display/shaderEffects/blend.vert
  nodes/display/shaderEffects/blend.frag
)
target_link_directories(CutieDesigner
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${LIB_TARGET_LINK_DIRECTORIES}
)

target_include_directories(CutieDesigner
  PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
  datatypes
  nodes
  nodes/display
  nodes/display/AType
  nodes/display/AType/Ukrug
  nodes/display/surface
  nodes/display/shaderEffects
  nodes/input
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

target_link_libraries(CutieDesigner
  PRIVATE Qt6::Core Qt6::Gui Qt6::Quick Qt6::QuickWidgets Qt6::Qml StatsModule NodeModule CutieUiModule vivid
)
target_compile_definitions(CutieDesigner PRIVATE GLM_ENABLE_EXPERIMENTAL)

install(TARGETS CutieDesigner
  BUNDLE DESTINATION .
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
