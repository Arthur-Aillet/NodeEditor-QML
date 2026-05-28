include(FetchContent)

FetchContent_Declare(vivid
    GIT_REPOSITORY  https://github.com/gurki/vivid
    GIT_TAG         c3476303cc749b241172ed101ec14defee268201
)

FetchContent_MakeAvailable(vivid)

set_property(TARGET vivid PROPERTY POSITION_INDEPENDENT_CODE ON)
