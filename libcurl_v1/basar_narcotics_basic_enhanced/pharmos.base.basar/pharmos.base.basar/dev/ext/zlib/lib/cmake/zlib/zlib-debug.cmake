#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "zlib::zlib" for configuration "Debug"
set_property(TARGET zlib::zlib APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(zlib::zlib PROPERTIES
  IMPORTED_IMPLIB_DEBUG "${_IMPORT_PREFIX}/lib/Debug/zlibd.lib"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/bin/Debug/zlibd.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS zlib::zlib )
list(APPEND _IMPORT_CHECK_FILES_FOR_zlib::zlib "${_IMPORT_PREFIX}/lib/Debug/zlibd.lib" "${_IMPORT_PREFIX}/bin/Debug/zlibd.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
