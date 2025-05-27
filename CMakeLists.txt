cmake_minimum_required(VERSION 3.16.2)
project(cpp_utils)
project(${PROJECT_NAME} VERSION 1.0.0)
set(CPP_UTILS_VERSION ${PROJECT_VERSION})

add_compile_options(-std=c++17)
set(CMAKE_BUILD_TYPE Release)
# set(CMAKE_BUILD_TYPE Debug)
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    
else()
    set(OPTIMIZATION_FLAGS $ENV{CPP_OPTIMIZATIONS})
    set(
      CMAKE_CXX_FLAGS 
      "${CMAKE_CXX_FLAGS} ${OPTIMIZATION_FLAGS} -fPIC"
    )
endif()

include(cmake/ccache.cmake)

link_directories(/usr/local/lib)
if(POLICY CMP0146)
  cmake_policy(SET CMP0146 OLD)
endif()
set(OPENCV_VERSION $ENV{OPENCV_VERSION})
message(STATUS "OpenCV version: ${OPENCV_VERSION}")
find_package(OpenCV ${OPENCV_VERSION} REQUIRED)

include_directories( 
  include
)

# ============ LIBRARY ============

add_library(${PROJECT_NAME} STATIC
  src/StageBase.cpp
  src/jsontools.cpp
  src/clitools.cpp
  src/opencvtools.cpp
  src/utils.cpp
)
target_include_directories(
  ${PROJECT_NAME} PUBLIC 
  ${OpenCV_INCLUDE_DIRS} 
  ${CUDA_INCLUDE_DIRS} 
)

target_link_libraries(${PROJECT_NAME} PUBLIC 
  ${CMAKE_THREAD_LIBS_INIT} 
  ${OpenCV_LIBS}
)


# ============ INSTALL ============
include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
  EXPORT ${PROJECT_NAME}Targets
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  INCLUDES DESTINATION include
)
# installs the export file (the “recipe”) created in the previous step to standard location within your install tree (for find_package)
install(EXPORT ${PROJECT_NAME}Targets
  NAMESPACE ${PROJECT_NAME}::
  DESTINATION lib/cmake/${PROJECT_NAME}
)

include(CMakePackageConfigHelpers)
configure_package_config_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Config.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
  INSTALL_DESTINATION lib/cmake/${PROJECT_NAME}
)
write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
  VERSION 1.0.0
  COMPATIBILITY AnyNewerVersion
)
# build tree usage
export(EXPORT ${PROJECT_NAME}Targets
  FILE "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Targets.cmake"
  NAMESPACE ${PROJECT_NAME}::
)

install(FILES
  "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
  DESTINATION lib/cmake/${PROJECT_NAME}
)
# Installs all header files (*.h) from your include/ folder to: ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}
install(DIRECTORY include/ 
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}
  FILES_MATCHING PATTERN "*.hpp" PATTERN "*.h"
) 
