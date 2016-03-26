message("+-------------------------------+")

message("* BUILD OPTIONS *")

message("* JASM System Monitor *")
message("* Copyright: ${COPYRIGHT} (Alessandro Spallina)")

if(UNIX)
    message("* OS: UNIX/Unix-like ")
elseif(WIN32)
    message("* OS: MS Windows ")
else()
    message("* OS: Unknown")
endif()

if(NOT BUILDARCH)
    set(BUILDARCH "native")
endif()

if(NOT WARNALL)
    set(WARNALL "no")
endif()

if(NOT WNOEXCLUDE)
    set(WNOEXCLUDE "no")
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message("* Build type: Debug ")
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    message("* Build type: Release")
endif()

if(WARNALL STREQUAL "yes")
    message("* WARNALL: yes")
elseif(WARNALL STREQUAL "no")
    message("* WARNALL: no")
    set(WNOEXCLUDE "no")
endif()

if(WNOEXCLUDE STREQUAL "yes")
    message("* WNOEXCLUDE: yes")
elseif(WNOEXCLUDE STREQUAL "no")
    message("* WNOEXCLUDE: no")
endif()

message("* C Compiler: ${CMAKE_C_COMPILER}")
message("* Architecture: ${BUILDARCH}")
message("* Build version: ${JASM_VERSION}")
message("* Build date: ${BUILD_DATE}")
message("* Source directory: ${PROJECT_SOURCE_DIR}")
message("* Install prefix: ${CMAKE_INSTALL_PREFIX}")
message("* Install directory: ${CMAKE_INSTALL_PREFIX}/bin")

message("+-------------------------------+")
