#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "wxStEditLib" for configuration "Debug"
set_property(TARGET wxStEditLib APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(wxStEditLib PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "CXX"
  IMPORTED_LINK_INTERFACE_LIBRARIES_DEBUG "F:/proj/cpp/libs/wxWidgets/lib/vc_lib/wxscintillad.lib;F:/proj/cpp/libs/wxWidgets/lib/vc_lib/wxpngd.lib;F:/proj/cpp/libs/wxWidgets/lib/vc_lib/wxtiffd.lib;F:/proj/cpp/libs/wxWidgets/lib/vc_lib/wxjpegd.lib;F:/proj/cpp/libs/wxWidgets/lib/vc_lib/wxzlibd.lib;F:/proj/cpp/libs/wxWidgets/lib/vc_lib/wxregexud.lib;F:/proj/cpp/libs/wxWidgets/lib/vc_lib/wxexpatd.lib;opengl32;glu32;winmm;comctl32;rpcrt4;wsock32"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/wxstedit-wx31d-1.6.0.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS wxStEditLib )
list(APPEND _IMPORT_CHECK_FILES_FOR_wxStEditLib "${_IMPORT_PREFIX}/lib/wxstedit-wx31d-1.6.0.lib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
