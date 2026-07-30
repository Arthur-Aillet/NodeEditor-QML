FetchContent_Declare(qtavplayer
    GIT_REPOSITORY  https://github.com/valbok/QtAVPlayer
    GIT_TAG         f7947fd081b46feeb8d7149d0265774cd0cb2d6f
)


list(APPEND CMAKE_LIBRARY_PATH "/usr/lib/x86_64-linux-gnu/")

FetchContent_GetProperties(qtavplayer)
if(NOT qtavplayer_POPULATED)
 FetchContent_MakeAvailable(qtavplayer)
endif()

set(QT_AVPLAYER_DIR "${CMAKE_SOURCE_DIR}/lib/qtavplayer-src/src/QtAVPlayer" CACHE PATH "Path to QtAVPlayer source root")
include(${QT_AVPLAYER_DIR}/QtAVPlayer.cmake)

set(QT_AVPLAYER_SOURCES "${QtAVPlayer_SOURCES}" CACHE STRING "Sources")
set(QT_AVPLAYER_LIBS "${QtAVPlayer_LIBS}" CACHE STRING "Libs")


include_directories("${QT_AVPLAYER_DIR}/..")
include_directories("${QT_AVPLAYER_DIR}")
