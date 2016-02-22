#Stefano Belli <stefano9913@gmail.com> <plus.google.com/+StefanoBelli>
#<https://github.com/StefanoBelli>

# --> ./CMakeLists.txt

#CMake 2.8 required (at least)
cmake_minimum_required(VERSION 2.8.12)

#JASM target
project(JASM)

#${PROJECT_SOURCE_DIR} & ${PROJECT_BINARY_DIR}

#Versioning & Build Date

#to-check
#PATTERN : MAJOR.MINOR.REVISION:PATCH
set(JASM_VMAJOR 0)
set(JASM_VMINOR 0)
set(JASM_VREV 0)
set(JASM_VPATCH 0)
set(JASM_VERSION "${JASM_VMAJOR}.${JASM_VMINOR}.${JASM_VREV}:${JASM_VPATCH}")
#if(${CMAKE_BUILD_TYPE})
#    set(JASM_VERSION "${JASM_VMAJOR}.${JASM_VMINOR}.${JASM_VREV}:${JASM_VPATCH}-debug")
#endif()

#Go to subdirectory
add_subdirectory(src/main/jasm)
