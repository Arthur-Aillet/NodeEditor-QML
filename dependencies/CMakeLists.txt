include(FetchContent)

FetchContent_Declare(vivid
    GIT_REPOSITORY  https://github.com/gurki/vivid
    GIT_TAG         v3.1.0
)

FetchContent_MakeAvailable(vivid)

set_property(TARGET vivid PROPERTY POSITION_INDEPENDENT_CODE ON)