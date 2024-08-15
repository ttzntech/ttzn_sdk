cmake_minimum_required(VERSION 3.5)

# Define the variable to specify the target platform
set(BUILD_ARCH "amd64" CACHE STRING "Specify the target platform (e.g. amd64, arm64)")

# Conditionally include the toolchain file based on the target platform
if(BUILD_ARCH STREQUAL "amd64")
    set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/cmake/amd64_toolchain.cmake)
elseif(BUILD_ARCH STREQUAL "arm64")
    set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/cmake/arm64_toolchain.cmake)
endif()

# Include the toolchain file if it was set
if(CMAKE_TOOLCHAIN_FILE)
    include(${CMAKE_TOOLCHAIN_FILE})
endif()

project(ttzn_sdk VERSION 0.1.1)

# CPP compile standard default is C++11
add_compile_options(-std=c++11)

file(GLOB CODY_SRCS src/cody/*)

add_library(${PROJECT_NAME} 
            # miscellaneous
            src/utils.cpp
            src/common.cpp
            # include CODY source code
            ${CODY_SRCS}
)

target_include_directories(${PROJECT_NAME}
    PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc/>
    $<INSTALL_INTERFACE:include/>
)

####### TESTING #######
option(ENABLE_TEST "enable test or not" OFF)

####### Demo #######
option(CODY_DEMO "compile cody demo code" OFF)

###### cody Demo ######
if (CODY_DEMO)
add_executable(cody_demo demo/cody_demo.cpp)
target_link_libraries(cody_demo ${PROJECT_NAME})
endif()

##### EXPORT AND INSTALL #####
message(STATUS "Project will be installed to ${CMAKE_INSTALL_PREFIX} with 'make install'")

set(DEF_INSTALL_CMAKEDIR share/cmake/${PROJECT_NAME})

include(GNUInstallDirs)
# Offer the user the choice of overriding the installation directories
set(INSTALL_LIBDIR ${CMAKE_INSTALL_LIBDIR} CACHE PATH "Installation directory for libraries")
set(INSTALL_BINDIR ${CMAKE_INSTALL_BINDIR} CACHE PATH "Installation directory for executables")
set(INSTALL_INCLUDEDIR ${CMAKE_INSTALL_INCLUDEDIR} CACHE PATH "Installation directory for header files")
set(INSTALL_CMAKEDIR ${DEF_INSTALL_CMAKEDIR} CACHE PATH "Installation directory for CMake files")

# Report to user
foreach(p LIB BIN INCLUDE CMAKE)
    file(TO_NATIVE_PATH ${CMAKE_INSTALL_PREFIX}/${INSTALL_${p}DIR} _path)
    message(STATUS " - To install ${p} components to ${_path}")
    unset(_path)
endforeach()

# Install target and export it
install(TARGETS ${PROJECT_NAME}
        EXPORT ${PROJECT_NAME}Targets
        LIBRARY DESTINATION ${INSTALL_LIBDIR} 
        ARCHIVE DESTINATION ${INSTALL_LIBDIR} 
        RUNTIME DESTINATION ${INSTALL_BINDIR} 
        INCLUDES DESTINATION ${INSTALL_INCLUDEDIR})

# Install header file
install(DIRECTORY inc/${PROJECT_NAME} DESTINATION ${INSTALL_INCLUDEDIR})

# Install scripts
file(GLOB SCRIPTS scripts/*)
install(PROGRAMS ${SCRIPTS} DESTINATION ${INSTALL_BINDIR})

# Export target configuration 
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    ${PROJECT_NAME}ConfigVersion.cmake
    VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}"
    COMPATIBILITY SameMajorVersion)

install(EXPORT ${PROJECT_NAME}Targets
    FILE ${PROJECT_NAME}Targets.cmake
    DESTINATION ${INSTALL_CMAKEDIR})

configure_file(cmake/${PROJECT_NAME}Config.cmake.in ${PROJECT_NAME}Config.cmake @ONLY)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
            "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
        DESTINATION ${INSTALL_CMAKEDIR})

# Packaging support
set(CPACK_PACKAGE_VENDOR "TTZNTech")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "TTZN SDK for Robots from Tiante Intelligent Technology")
set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}_${CPACK_PACKAGE_VERSION}_${CMAKE_SYSTEM_PROCESSOR}")

set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README.md")

set(CPACK_GENERATOR "DEB")
set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE ${CMAKE_SYSTEM_PROCESSOR})
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Joonchen Liau (liaojuncheng123@foxmail.com)") 
set(CPACK_SOURCE_IGNORE_FILES .git .*build.* .vscode)

# Ensure CPack uses the overridden installation directories
set(CPACK_PACKAGING_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX})

include(CPack)
