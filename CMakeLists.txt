#Stefano Belli <stefano9913@gmail.com> <plus.google.com/+StefanoBelli>
#<https://github.com/StefanoBelli>

#Suggested parameters: -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_COMPILER=/usr/bin/gcc (or export environment variable CC=/usr/bin/gcc)

# --> ./CMakeLists.txt

#CMake 2.8 required (at least)
cmake_minimum_required(VERSION 2.8.12)

#JASM target
project(JASM)

set(COPYRIGHT "GNU Public License 3 - GPLv3")

#Versioning & Build Date

#PATTERN : MAJOR.MINOR.REVISION:PATCH
#Ex : X.Y.Z:P-btype-rel

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
