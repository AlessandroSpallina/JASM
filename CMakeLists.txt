#Stefano Belli <stefano9913@gmail.com> <plus.google.com/+StefanoBelli>
#<https://github.com/StefanoBelli>

#Suggested parameters: -DCMAKE_BUILD_TYPE=Release
#                      -DCMAKE_C_COMPILER=/usr/bin/gcc (or export environment variable CC=/usr/bin/gcc)
#                      -DCMAKE_INSTALL_PREFIX=/usr/local (if you change this, you must change jasm.service)
#(if package distributor)
#                      -DCPACK_SYSTEM_NAME=$(uname)-$(uname -m)
#                      -DCPACK_PACKAGE_VENDOR="Your Name"
#(if package distributor::debian)
#                      -DCPACK_DEBIAN_PACKAGE_ARCHITECTURE=i386 (change this value)
#                      -DCPACK_DEBIAN_PACKAGE_MAINTAINER="Name Surname"
#(if package distributor::rpm)
#                      -DCPACK_RPM_PACKAGE_ARCHITECTURE=i366 (change this value)


# --> ./CMakeLists.txt

include(InstallRequiredSystemLibraries)

#CMake 2.8 required (at least)
cmake_minimum_required(VERSION 2.8.12)

#JASM target
project(JASM)

#If no build type is specified
if(NOT CMAKE_BUILD_TYPE)
		set(CMAKE_BUILD_TYPE "Release")
endif()

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
#set(JASM_VPATCH 0)
set(JASM_BTYPE develstage)
# IMPORTANT, CHANGE THIS FOR EVERY NEW VERSION #

# DO NOT EDIT BELOW #
if(CMAKE_BUILD_TYPE STREQUAL "Debug" )
                set(JASM_VERSION "${JASM_VMAJOR}.${JASM_VMINOR}.${JASM_VREV}-${JASM_BTYPE}-debug")
else()
                set(JASM_VERSION "${JASM_VMAJOR}.${JASM_VMINOR}.${JASM_VREV}-${JASM_BTYPE}-release")
endif()
# DO NOT EDIT ABOVE #

# Build date [DO NOT EDIT] #
string(TIMESTAMP BUILD_DATE "%Y/%m/%d %H:%M:%S")
# Build date [DO NOT EDIT] #

#Go to subdirectory
add_subdirectory(src/main/jasm)

#Installation files
#bin/jasm -> /usr/local/bin/jasm
install(FILES ${PROJECT_SOURCE_DIR}/bin/jasm
    PERMISSIONS WORLD_EXECUTE
    DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
#jasm.service -> /etc/systemd/system/jasm.service
install(FILES ${PROJECT_SOURCE_DIR}/jasm.service
    PERMISSIONS WORLD_READ
    DESTINATION /etc/systemd/system)

#Packaging#
set(CPACK_GENERATOR "DEB;RPM")

#General
set(CPACK_PACKAGE_VERSION_MAJOR ${JASM_VMAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${JASM_VMINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${JASM_VREV})
set(JVERSION ${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH})
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Modular & Hackable System Monitor")
set(CPACK_PACKAGE_DESCRIPTION "JASM is another system monitoring platform... But you can change it your own!")
set(CPACK_PACKAGE_DESCRIPTION "Modular System Monitor")
set(CPACK_PACKAGE_CONTACT "jasm-team.slack.com")
set(CPACK_PACKAGE_NAME "JASM")
set(CPACK_PACKAGE_FILE_NAME "jasm-${JVERSION}-${CPACK_SYSTEM_NAME}")
set(CPACK_RESOURCE_FILE_LICENSE "${PROJECT_SOURCE_DIR}/LICENSE")
set(CPACK_RESOURCE_FILE_README "${PROJECT_SOURCE_DIR}/README.md")
set(CPACK_PACKAGE_EXECUTABLES "${PROJECT_SOURCE_DIR}/bin/jasm")

#Debian
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "JASM team")
set(CPACK_DEBIAN_PACKAGE_SECTION "admin")
set(CPACK_DEBIAN_PACKAGE_DESCRIPTION "JASM is an hackable and modular system monitor, change it your own")
set(CPACK_DEBIAN_PACKAGE_DESCRIPTION_SUMMARY "Modular and Hackable System Monitor")

#RH
set(CPACK_RPM_PACKAGE_LICENSE "GPLv3")
set(CPACK_RPM_COMPRESSION_TYPE "xz")
set(CPACK_RPM_PACKAGE_RELEASE 0)
set(CPACK_RPM_PACKAGE_URL "https://github.com/AlessandroSpallina/JASM")
set(CPACK_RPM_PACKAGE_DESCRIPTION "JASM is another system monitoring platform... But you can change it your own!")

#Use CPack
include(CPack)

#Show options finally
include(cmake/showsettings.cmake)