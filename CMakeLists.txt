#Stefano Belli <stefano9913@gmail.com> <plus.google.com/+StefanoBelli>
#<https://github.com/StefanoBelli>

#Suggested parameters: -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_COMPILER=/usr/bin/gcc (or export environment variable CC=/usr/bin/gcc)

# --> ./CMakeLists.txt

include(InstallRequiredSystemLibraries)

#CMake 2.8 required (at least)
cmake_minimum_required(VERSION 2.8.12)

#JASM target
project(JASM)

set(MAINDIR ${PROJECT_SOURCE_DIR})

set(COPYRIGHT "GNU Public License 3 - GPLv3")

#Versioning & Build Date

#PATTERN : MAJOR.MINOR.REVISION:PATCH-BType-reltype
#Ex : X.Y.Z:P-btype-rel
#     0.0.1:3-unstable-release

# IMPORTANT, CHANGE THIS FOR EVERY NEW VERSION #
set(JASM_VMAJOR 0)
set(JASM_VMINOR 0)
set(JASM_VREV 1)
set(JASM_VPATCH 0)
set(JASM_BTYPE develstage)
# IMPORTANT, CHANGE THIS FOR EVERY NEW VERSION #

# DO NOT EDIT BELOW #
if(CMAKE_BUILD_TYPE STREQUAL "Debug" )
		set(JASM_VERSION "${JASM_VMAJOR}.${JASM_VMINOR}.${JASM_VREV}:${JASM_VPATCH}-${JASM_BTYPE}-debug")
else()
		set(JASM_VERSION "${JASM_VMAJOR}.${JASM_VMINOR}.${JASM_VREV}:${JASM_VPATCH}-${JASM_BTYPE}-release")
endif()
# DO NOT EDIT ABOVE #

# Build date [DO NOT EDIT] #
string(TIMESTAMP BUILD_DATE "%Y/%m/%d %H:%M:%S")
# Build date [DO NOT EDIT] #

#Go to subdirectory
add_subdirectory(src/main/jasm)

install(FILES ${PROJECT_SOURCE_DIR}/bin/jasm
    PERMISSIONS WORLD_EXECUTE
    DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
install(FILES ${PROJECT_SOURCE_DIR}/jasm.service
    PERMISSIONS WORLD_READ
    DESTINATION /etc/systemd/system)

set(CPACK_GENERATOR "DEB")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Ste")

#Use CPack
include(CPack)

