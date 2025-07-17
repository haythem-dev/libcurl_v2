# zlib-config.cmake - package configuration file

set(PACKAGE_VERSION 1.2.11)
set(zlib_VERSION 1.2.11)

include(${CMAKE_CURRENT_LIST_DIR}/zlib.cmake)

get_filename_component(ZLIB_INCLUDE_DIRS "${CMAKE_CURRENT_LIST_FILE}" PATH)
get_filename_component(ZLIB_INCLUDE_DIRS "${ZLIB_INCLUDE_DIRS}" PATH)
get_filename_component(ZLIB_INCLUDE_DIRS "${ZLIB_INCLUDE_DIRS}" PATH)
get_filename_component(ZLIB_INCLUDE_DIRS "${ZLIB_INCLUDE_DIRS}" PATH)
if(ZLIB_INCLUDE_DIRS STREQUAL "/")
  set(ZLIB_INCLUDE_DIRS "")
endif()

set(ZLIB_INCLUDE_DIRS ${ZLIB_INCLUDE_DIRS}/include)
set(ZLIB_LIBRARIES zlib::zlib)

set_target_properties(zlib::zlib
    PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES ${ZLIB_INCLUDE_DIRS}
)
