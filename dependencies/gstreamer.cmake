include(FindPkgConfig)
if(!${PKG_CONFIG_FOUND})
  MESSAGE(FATAL_ERROR "pkg-config required. Please install it")
  return ("Error - pkg-config not found")
endif()

find_package(PkgConfig REQUIRED)
pkg_check_modules(GSTREAMER REQUIRED gstreamer-1.0>=1.6 gstreamer-video-1.0>=1.6 gstreamer-plugins-base-1.0 gstreamer-plugins-good-1.0 gstreamer-plugins-bad-1.0 gstreamer-gl-1.0) 
add_definitions(-DGST_USE_UNSTABLE_API)