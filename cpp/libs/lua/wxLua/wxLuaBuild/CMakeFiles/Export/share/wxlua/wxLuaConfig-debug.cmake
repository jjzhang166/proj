#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "LuaLib" for configuration "Debug"
set_property(TARGET LuaLib APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(LuaLib PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "C"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/wxlua_lua51-wx31d-2.8.12.3.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS LuaLib )
list(APPEND _IMPORT_CHECK_FILES_FOR_LuaLib "${_IMPORT_PREFIX}/lib/wxlua_lua51-wx31d-2.8.12.3.lib" )

# Import target "wxLuaBindLib" for configuration "Debug"
set_property(TARGET wxLuaBindLib APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(wxLuaBindLib PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "CXX"
  IMPORTED_LINK_INTERFACE_LIBRARIES_DEBUG "wxLuaLib"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/wxlua_bind-wx31d-2.8.12.3.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS wxLuaBindLib )
list(APPEND _IMPORT_CHECK_FILES_FOR_wxLuaBindLib "${_IMPORT_PREFIX}/lib/wxlua_bind-wx31d-2.8.12.3.lib" )

# Import target "wxLuaLib" for configuration "Debug"
set_property(TARGET wxLuaLib APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(wxLuaLib PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "C;CXX"
  IMPORTED_LINK_INTERFACE_LIBRARIES_DEBUG "LuaLib;F:/proj/cpp/libs/wxWidgets/lib/vc_lib/wxscintillad.lib;F:/proj/cpp/libs/wxWidgets/lib/vc_lib/wxpngd.lib;F:/proj/cpp/libs/wxWidgets/lib/vc_lib/wxtiffd.lib;F:/proj/cpp/libs/wxWidgets/lib/vc_lib/wxjpegd.lib;F:/proj/cpp/libs/wxWidgets/lib/vc_lib/wxzlibd.lib;F:/proj/cpp/libs/wxWidgets/lib/vc_lib/wxregexud.lib;F:/proj/cpp/libs/wxWidgets/lib/vc_lib/wxexpatd.lib;opengl32;glu32;winmm;comctl32;rpcrt4;wsock32"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/wxlua-wx31d-2.8.12.3.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS wxLuaLib )
list(APPEND _IMPORT_CHECK_FILES_FOR_wxLuaLib "${_IMPORT_PREFIX}/lib/wxlua-wx31d-2.8.12.3.lib" )

# Import target "wxLuaDebugLib" for configuration "Debug"
set_property(TARGET wxLuaDebugLib APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(wxLuaDebugLib PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "CXX"
  IMPORTED_LINK_INTERFACE_LIBRARIES_DEBUG "wxLuaLib"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/wxlua_debug-wx31d-2.8.12.3.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS wxLuaDebugLib )
list(APPEND _IMPORT_CHECK_FILES_FOR_wxLuaDebugLib "${_IMPORT_PREFIX}/lib/wxlua_debug-wx31d-2.8.12.3.lib" )

# Import target "wxLuaDebuggerLib" for configuration "Debug"
set_property(TARGET wxLuaDebuggerLib APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(wxLuaDebuggerLib PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "CXX"
  IMPORTED_LINK_INTERFACE_LIBRARIES_DEBUG "wxLuaBindLib;wxLuaDebugLib"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/wxlua_debugger-wx31d-2.8.12.3.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS wxLuaDebuggerLib )
list(APPEND _IMPORT_CHECK_FILES_FOR_wxLuaDebuggerLib "${_IMPORT_PREFIX}/lib/wxlua_debugger-wx31d-2.8.12.3.lib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
