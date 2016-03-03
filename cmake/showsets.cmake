message("+-------------------------------+")

message("* BUILD OPTIONS *")

message("* JASM System Monitor *")
message("* Copyright: ${COPYRIGHT} (Alessandro Spallina)")

if(UNIX)
		message("* OS: UNIX/Unix-like ")
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
		message("* Build type: Debug ")
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
		message("* Build type: Release")
endif()

if(WARNALL STREQUAL "yes")
		message("* All warning will be produced")
endif()

message("* Build version: ${JASM_VERSION}")
message("* Build date: ${BUILD_DATE}")
message("* Source directory: ${PROJECT_SOURCE_DIR}")
message("* Install prefix: ${CMAKE_INSTALL_PREFIX}")
message("* Install directory: ${CMAKE_INSTALL_PREFIX}/bin")
message("")
message("")
message("+-------------------------------+")
