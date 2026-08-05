qt_add_qml_module(time
  URI CutieDesigner.Time
  SHARED
  SOURCES
  TimeController.hpp
  TimeController.cpp
)

target_include_directories(time PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_directories(time
  PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${LIB_TARGET_LINK_DIRECTORIES}
)

target_link_libraries(time PUBLIC
  Qt6::Core
  Qt6::QuickWidgets
  Qt6::Concurrent
  CutieUiModule
)

