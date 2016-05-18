# Stefano Belli
# --> src/main/jasm/CMakeLists.txt
message(">>> CMake: src/main/jasm/CMakeLists.txt")
cmake_minimum_required(VERSION 2.8.12) #CMake 2.8

project(JASM C) #JASM

configure_file (
	"${PROJECT_SOURCE_DIR}/core/jasmbuild_info.h.in"
	"${PROJECT_SOURCE_DIR}/core/jasmbuild_info.h"
    )

find_package(Threads REQUIRED) 

if(NOT WARNALL)
    message("-- WARNALL NOT SET -- no ")
    set(WARNALL "no")
    if(WNOEXCLUDE)
        set(WNOEXCLUDE "no")
        message("-- NOTICE: If WARNALL is not set, you cannot set WNOEXCLUDE")
    endif()
endif()

if(NOT CMAKE_C_COMPILER)
    if(NOT DEFINED $ENV{CC})
        set(CMAKE_C_COMPILER "/usr/bin/cc")
    endif()
endif()

if(NOT MTUNESW)
    message("-- MTUNESW NOT SET -- native")
    set(MTUNESW "native")
endif()

if(NOT WNOEXCLUDE)
    message("-- WNOEXCLUDE NOT SET -- no ")
    set(WNOEXCLUDE "no")
endif()

if(NOT BUILDARCH)
    message("-- BUILDARCH NOT SET -- using default")
    set(BUILDARCH "")
else()
    message("-- BUILDARCH SET -- ${BUILDARCH}")
    set(BUILDARCH "-m${BUILDARCH}")
endif()

if(NOT CFLAGSADD)
    message("-- CFLAGSADD NOT SET -- using just default")
    set(CFLAGSADD "")
endif()

message("-- Setting CFLAGS ")
set(CMAKE_C_FLAGS_RELEASE "${CFLAGSADD} -O2 -pipe ${BUILDARCH} -mtune=${MTUNESW} -std=c11 ")
set(CMAKE_C_FLAGS_DEBUG "${CFLAGSADD} -DDEBUG -g ${BUILDARCH} -mtune=${MTUNESW} -std=c11 -Wall ")

if(WARNALL STREQUAL "yes")
    message("-- WARNALL ENABLED ")
    if(WNOEXCLUDE STREQUAL "yes")
        message("-- WNOEXCLUDE ENABLED ")
        set(EXCLUDED_WARNS " ")
    elseif(WNOEXCLUDE STREQUAL "no")
        message("-- WNOEXCLUDE DISABLED")
        set(EXCLUDED_WARNS "-Wno-sign-compare ")
    endif()
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -W -Wextra -Wshadow ${EXCLUDED_WARNS}")
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Wall -W -Wextra -Wshadow ${EXCLUDED_WARNS}")
elseif(WARNALL STREQUAL "no")
    message("-- WARNALL DISABLED")
    if(WNOEXCLUDE)
        set(WNOEXCLUDE "no")
        message("-- NOTICE: If WARNALL is set as no, you cannot set WNOEXCLUDE")
    endif()
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(BUILDFLAGS ${CMAKE_C_FLAGS_RELEASE})
elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(BUILDFLAGS ${CMAKE_C_FLAGS_DEBUG})
endif()

message("-- Looking for sources...")

file(GLOB_RECURSE CORESRCS "core/*.c")
file(GLOB_RECURSE MODSRCS "modules/*.c")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG  ${MAINDIR}/bin-debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${MAINDIR}/bin)

#
# __BSD_SOURCE : required for FreeBSD
#
add_definitions(-D_XOPEN_SOURCE=700 -D_GNU_SOURCE -D__BSD_VISIBLE)

if(CORESRCS AND MODSRCS)
    add_library(jasmcore STATIC ${CORESRCS})#libjasmcore.a (linked)
    add_library(jasmmodules STATIC ${MODSRCS})#libjasmmodules.a (linked)
    add_executable(jasm core/jasm.c) 
    target_link_libraries(jasm jasmcore jasmmodules ${CMAKE_THREAD_LIBS_INIT})
else()
    message("-- Error: Core sources and Modules sources not found!!")
endif()

message("<<< CMake: src/main/jasm/CMakeLists.txt ; DONE!")
message("* BUILDFLAGS: ${BUILDFLAGS}")
