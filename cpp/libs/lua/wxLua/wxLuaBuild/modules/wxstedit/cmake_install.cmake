# Install script for directory: F:/proj/lua/wxLua/modules/wxstedit

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/wxLua")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "F:/proj/lua/wxLua/wxLuaBuild/lib/Debug/wxstedit-wx31d-1.6.0.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "F:/proj/lua/wxLua/wxLuaBuild/lib/Release/wxstedit-wx31-1.6.0.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "F:/proj/lua/wxLua/wxLuaBuild/lib/MinSizeRel/wxstedit-wx31-1.6.0.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "F:/proj/lua/wxLua/wxLuaBuild/lib/RelWithDebInfo/wxstedit-wx31-1.6.0.lib")
  endif()
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/wxstedit/wxStEditConfig.cmake")
    file(DIFFERENT EXPORT_FILE_CHANGED FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/wxstedit/wxStEditConfig.cmake"
         "F:/proj/lua/wxLua/wxLuaBuild/modules/wxstedit/CMakeFiles/Export/share/wxstedit/wxStEditConfig.cmake")
    if(EXPORT_FILE_CHANGED)
      file(GLOB OLD_CONFIG_FILES "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/wxstedit/wxStEditConfig-*.cmake")
      if(OLD_CONFIG_FILES)
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/wxstedit/wxStEditConfig.cmake\" will be replaced.  Removing files [${OLD_CONFIG_FILES}].")
        file(REMOVE ${OLD_CONFIG_FILES})
      endif()
    endif()
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/wxstedit" TYPE FILE FILES "F:/proj/lua/wxLua/wxLuaBuild/modules/wxstedit/CMakeFiles/Export/share/wxstedit/wxStEditConfig.cmake")
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/wxstedit" TYPE FILE FILES "F:/proj/lua/wxLua/wxLuaBuild/modules/wxstedit/CMakeFiles/Export/share/wxstedit/wxStEditConfig-debug.cmake")
  endif()
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/wxstedit" TYPE FILE FILES "F:/proj/lua/wxLua/wxLuaBuild/modules/wxstedit/CMakeFiles/Export/share/wxstedit/wxStEditConfig-minsizerel.cmake")
  endif()
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/wxstedit" TYPE FILE FILES "F:/proj/lua/wxLua/wxLuaBuild/modules/wxstedit/CMakeFiles/Export/share/wxstedit/wxStEditConfig-relwithdebinfo.cmake")
  endif()
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/wxstedit" TYPE FILE FILES "F:/proj/lua/wxLua/wxLuaBuild/modules/wxstedit/CMakeFiles/Export/share/wxstedit/wxStEditConfig-release.cmake")
  endif()
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/doc/wxStEdit" TYPE FILE FILES
    "F:/proj/lua/wxLua/modules/wxstedit/docs/ChangeLog.txt"
    "F:/proj/lua/wxLua/modules/wxstedit/docs/readme.txt"
    "F:/proj/lua/wxLua/modules/wxstedit/docs/readme.htm"
    "F:/proj/lua/wxLua/modules/wxstedit/docs/wxstedit.htm"
    )
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/wx/stedit" TYPE FILE FILES
    "F:/proj/lua/wxLua/wxLuaBuild/include/wx/stedit/setup.h"
    "F:/proj/lua/wxLua/modules/wxstedit/include/wx/stedit/pairarr.h"
    "F:/proj/lua/wxLua/modules/wxstedit/include/wx/stedit/setup0.h"
    "F:/proj/lua/wxLua/modules/wxstedit/include/wx/stedit/steart.h"
    "F:/proj/lua/wxLua/modules/wxstedit/include/wx/stedit/stedefs.h"
    "F:/proj/lua/wxLua/modules/wxstedit/include/wx/stedit/stedit.h"
    "F:/proj/lua/wxLua/modules/wxstedit/include/wx/stedit/stedlgs.h"
    "F:/proj/lua/wxLua/modules/wxstedit/include/wx/stedit/steevent.h"
    "F:/proj/lua/wxLua/modules/wxstedit/include/wx/stedit/steexprt.h"
    "F:/proj/lua/wxLua/modules/wxstedit/include/wx/stedit/stefindr.h"
    "F:/proj/lua/wxLua/modules/wxstedit/include/wx/stedit/steframe.h"
    "F:/proj/lua/wxLua/modules/wxstedit/include/wx/stedit/stelangs.h"
    "F:/proj/lua/wxLua/modules/wxstedit/include/wx/stedit/stemenum.h"
    "F:/proj/lua/wxLua/modules/wxstedit/include/wx/stedit/stenoteb.h"
    "F:/proj/lua/wxLua/modules/wxstedit/include/wx/stedit/steopts.h"
    "F:/proj/lua/wxLua/modules/wxstedit/include/wx/stedit/steprefs.h"
    "F:/proj/lua/wxLua/modules/wxstedit/include/wx/stedit/steprint.h"
    "F:/proj/lua/wxLua/modules/wxstedit/include/wx/stedit/steshell.h"
    "F:/proj/lua/wxLua/modules/wxstedit/include/wx/stedit/stesplit.h"
    "F:/proj/lua/wxLua/modules/wxstedit/include/wx/stedit/stestyls.h"
    "F:/proj/lua/wxLua/modules/wxstedit/include/wx/stedit/stetree.h"
    )
endif()

