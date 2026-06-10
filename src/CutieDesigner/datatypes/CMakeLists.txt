add_library(datatypes
  SHARED
  SurfaceData.cpp
  TextData.cpp
)

target_include_directories(datatypes PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(datatypes PUBLIC
  Qt6::Core
  Qt6::Gui
  NodeEditor
)