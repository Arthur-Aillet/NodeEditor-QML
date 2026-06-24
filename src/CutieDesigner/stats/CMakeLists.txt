qt_add_qml_module(stats
  STATIC
  URI CutieDesigner.Stats
  QML_FILES
  StatsView.qml
  SimpleFpsCounter.qml
  DEPENDENCIES QtQuick CutieUiModule
  SOURCES
  ./FrequencyMonitor/FrequencyMonitor.cpp
  ./FrequencyMonitor/FrequencyMonitor.hpp
  RESOURCES
  ./loading.svg
)

target_include_directories(stats PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${LIB_TARGET_LINK_DIRECTORIES}
  ./FrequencyMonitor
)

target_link_libraries(stats
  PUBLIC Qt::Core Qt::Gui Qt::Qml Qt::Quick Qt::QuickControls2 NodeEditor CutieUiModule
)
