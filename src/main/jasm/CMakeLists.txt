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
if(UNIX)
	if(THREADS_HAVE_PTHREAD_ARG)
		target_compile_options(PUBLIC jasm "-pthread")
	endif()
endif()

if(NOT WARNALL)
    message("-- WARNALL NOT SET -- no ")
    set(WARNALL "no")
endif()

if(NOT BUILDARCH)
    message("-- BUILDARCH NOT SET -- native")
    set(BUILDARCH "native")
endif()

message("-- Setting CFLAGS ")
set(CMAKE_C_FLAGS_RELEASE "-O2 -pipe -mtune=${BUILDARCH} -std=c11 -D_GNU_SOURCE")
set(CMAKE_C_FLAGS_DEBUG "-DDEBUG -g -mtune=${BUILDARCH} -std=c11 -Wall -D_GNU_SOURCE ")

if(WARNALL STREQUAL "yes")
    message("-- WARNALL ENABLED ")
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -W -Wextra -Wshadow")
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Wall -W -Wextra -Wshadow")
elseif(WARNALL STREQUAL "no")
    message("-- WARNALL DISABLED")
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(BUILDFLAGS ${CMAKE_C_FLAGS_RELEASE})
elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(BUILDFLAGS ${CMAKE_C_FLAGS_DEBUG})
endif()

message("-- Preparing sources...")

set(CORESRCS core/configfile.c core/getter.c core/ipc.c core/logger.c 
 core/miscellaneous.c core/modules.c core/modules_list.c core/signals.c)
set(MODSRCS modules/module_logsender.c)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG  ${MAINDIR}/bin-debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${MAINDIR}/bin)

#timedate definition
add_definitions(-D_XOPEN_SOURCE=700)

add_library(jasmcore STATIC ${CORESRCS})#libjasmcore.a (linked)
add_library(jasmmodules STATIC ${MODSRCS})#libjasmmodules.a (linked)
add_executable(jasm core/jasm.c) 

target_link_libraries(jasm jasmcore jasmmodules ${CMAKE_THREAD_LIBS_INIT})

message("<<< CMake: src/main/jasm/CMakeLists.txt ; DONE!")
message("* BUILDFLAGS: ${BUILDFLAGS}")
