add_subdirectory(NodeModule)
add_subdirectory(StatsModule)

qt_add_executable(CutieDesigner
  main.cpp
)

qt_add_qml_module(CutieDesigner
  URI CutieDesignerModule
  QML_FILES
  Main.qml
  objects/AType.qml
  TextDisplay.qml
  nodes/PortLabel.qml
  objects/UkrugCharacter.qml
  nodes/TextTyper/TextTyper.qml
  objects/Ukrug.qml

  SOURCES

  objects/UkrugPointsList.hpp
  objects/UkrugPointsList.cpp
  objects/ObjectLoader.cpp
  objects/ObjectLoader.cpp

  nodes/AdditionModel.hpp
  nodes/DivisionModel.hpp
  nodes/MathOperationDataModel.hpp
  nodes/SubtractionModel.hpp
  nodes/ValueNodeModel.cpp
  nodes/ValueNodeModel.hpp
  nodes/DecimalData.hpp
  nodes/TextData.hpp
  nodes/MathOperationDataModel.cpp
  nodes/MultiplicationModel.hpp

  nodes/TextTyper/TextTyperModel.hpp
  nodes/TextTyper/TextTyperModel.cpp

  nodes/Object/ATypeNode.cpp
  nodes/Object/ATypeNode.hpp
  nodes/Object/ObjectData.hpp
  nodes/Object/ObjectData.cpp
  nodes/Object/ObjectDisplayDataModel.cpp
  nodes/Object/ObjectDisplayDataModel.hpp

  nodes/TextTyper/TextTyperEventList.hpp
  nodes/TextTyper/TextTyperEventList.cpp

  DEPENDENCIES QtQuick QtQuick.Layout StatsModule NodeModule
  RESOURCES
  objects/ukrug.vert.qsb
  objects/ukrug.frag.qsb
  objects/UkrugPointsData.json
)

target_link_directories(CutieDesigner
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${LIB_TARGET_LINK_DIRECTORIES}
)

target_include_directories(CutieDesigner
  PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
  nodes
  nodes/Object
  nodes/TextTyper
  objects
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
  PRIVATE Qt6::Core Qt6::Gui Qt6::Quick Qt6::QuickWidgets Qt6::Qml StatsModule NodeModule vivid
)
target_compile_definitions(CutieDesigner PRIVATE GLM_ENABLE_EXPERIMENTAL)

install(TARGETS CutieDesigner
  BUNDLE DESTINATION .
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
