add_subdirectory(display)
add_subdirectory(input)
add_subdirectory(process)

add_library(nodes STATIC)

target_link_libraries(nodes PUBLIC
  display
  input
  process
)