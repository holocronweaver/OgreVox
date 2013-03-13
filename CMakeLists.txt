cmake_minimum_required(VERSION 2.8)

#set(APPLICATION_NAME "OgreVox")

project("OgreVox")

add_subdirectory(lib)

if(WIN32)
  set(OGRE_STATIC TRUE)
  set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++0x")
  set(CMAKE_MODULE_PATH "$ENV{OGRE_HOME}/CMake/;${CMAKE_MODULE_PATH}")
  set(OGRE_SAMPLES_INCLUDEPATH "$ENV{OGRE_HOME}/Samples/include")
endif(WIN32)

if(UNIX)
  set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++0x")
  if(EXISTS "/usr/local/lib/OGRE/cmake")
    set(CMAKE_MODULE_PATH "/usr/local/lib/OGRE/cmake/;${CMAKE_MODULE_PATH}")
    set(OGRE_SAMPLES_INCLUDEPATH "/usr/local/share/OGRE/samples/Common/include/")
  elseif(EXISTS "/usr/lib/OGRE/cmake")
    set(CMAKE_MODULE_PATH "/usr/lib/OGRE/cmake/;${CMAKE_MODULE_PATH}")
    set(OGRE_SAMPLES_INCLUDEPATH "/usr/share/OGRE/samples/Common/include/")
  else ()
    message(SEND_ERROR "Failed to find module path.")
  endif(EXISTS "/usr/local/lib/OGRE/cmake")
endif(UNIX)

if (CMAKE_BUILD_TYPE STREQUAL "")
  # CMake defaults to leaving CMAKE_BUILD_TYPE empty. This screws up
  # differentiation between debug and release builds.
  set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING "Choose the type of build, options are: None (CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif ()

set(CMAKE_DEBUG_POSTFIX "_d")
set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_BINARY_DIR}/dist")

find_package(OGRE REQUIRED)

# Add OgreTerrain.{so,dll,etc.} to our linked libraries
#   These variables are defined in FindOGRE.cmake, mine is
#   found at /usr/local/lib/OGRE/cmake/
# if(OGRE_Terrain_FOUND)
# # pretty sure the following include is unneeded
#   # include_directories(${OGRE_Terrain_INCLUDE_DIRS})
# # append OgreTerrain to the end of the OGRE_LIBRARIES variable
#   set(OGRE_LIBRARIES ${OGRE_LIBRARIES} ${OGRE_Terrain_LIBRARIES})  
#   message(STATUS "Found OGRE_Terrain: ${OGRE_Terrain_LIBRARIES}")
# else(OGRE_Terrain_FOUND)
#   message(SEND_ERROR "OgreTerrain Library not found.")
# endif(OGRE_Terrain_FOUND)


find_package(OIS REQUIRED)

if(NOT OIS_FOUND)
	message(SEND_ERROR "Failed to find OIS.")
endif()


# find_package(CEGUI REQUIRED)

# if(NOT CEGUI_FOUND)
#   message(SEND_ERROR "Failed to find CEGUI.")
# endif()

# find_package(CEGUIOGRE REQUIRED)

# if(NOT CEGUIOGRE_FOUND)
#   message(SEND_ERROR "Failed to find CEGUIOGRE.")
# endif()

find_package(PolyVox REQUIRED)

if(NOT PolyVox_FOUND)
  message(SEND_ERROR "Failed to find PolyVox.")
endif()

# Find Boost
# Prefer static linking in all cases
if (WIN32 OR APPLE)
	set(Boost_USE_STATIC_LIBS TRUE)
else ()
	# Statically linking boost to a dynamic Ogre build doesn't work on Linux 64bit
	set(Boost_USE_STATIC_LIBS ${OGRE_STATIC})
endif ()
if (APPLE AND OGRE_BUILD_PLATFORM_APPLE_IOS)
  set(Boost_USE_MULTITHREADED OFF)
endif()
set(Boost_ADDITIONAL_VERSIONS "1.53" "1.53.0" "1.52" "1.52.0" "1.51" "1.51.0" "1.50" "1.50.0" "1.49" "1.49.0" "1.48" "1.48.0" "1.47" "1.47.0" "1.46" "1.46.0" "1.45" "1.45.0" "1.44" "1.44.0" "1.42" "1.42.0" "1.41.0" "1.41" "1.40.0" "1.40")
# Components that need linking (NB does not include header-only components like bind)
set(OGRE_BOOST_COMPONENTS thread date_time)
find_package(Boost COMPONENTS ${OGRE_BOOST_COMPONENTS} QUIET)
if (NOT Boost_FOUND)
  # Try again with the other type of libs
  if(Boost_USE_STATIC_LIBS)
    set(Boost_USE_STATIC_LIBS OFF)
  else()
    set(Boost_USE_STATIC_LIBS ON)
  endif()
  find_package(Boost COMPONENTS ${OGRE_BOOST_COMPONENTS} QUIET)
endif()

if(Boost_FOUND AND Boost_VERSION GREATER 104900)
  set(OGRE_BOOST_COMPONENTS thread date_time system chrono)
  find_package(Boost COMPONENTS ${OGRE_BOOST_COMPONENTS} QUIET)
endif()

if(Boost_FOUND)
  # Set up referencing of Boost
  include_directories(${Boost_INCLUDE_DIR})
  add_definitions(-DBOOST_ALL_NO_LIB)
  set(OGRE_LIBRARIES ${OGRE_LIBRARIES} ${Boost_LIBRARIES})
endif()

set(HDRS
  ./BareOgre.h
  ./VoxelSphere.h
  ./LargeVoxelSphere.h
  ./VoxelTerrain.h
  ./VoxelPlanet.h
  ./Noise.h
  ${PROJECT_NAME}.h
)

set(SRCS
  ./BareOgre.cpp
  ./VoxelSphere.cpp
  ./LargeVoxelSphere.cpp
  ./VoxelTerrain.cpp
  ./VoxelPlanet.cpp
  ./Noise.cpp
  ${PROJECT_NAME}.cpp
)

include_directories( 
  ${OGRE_INCLUDE_DIRS}
  ${OGRE_SAMPLES_INCLUDEPATH}
  ${OIS_INCLUDE_DIRS}
  # ${CEGUI_INCLUDE_DIRS}
  # ${CEGUIOGRE_INCLUDE_DIRS}
  ${PolyVox_INCLUDE_DIRS}
  "${PROJECT_SOURCE_DIR}/lib/accidentalnoise/include"
  #${AccidentalNoise_INCLUDE_DIRS}
)

add_executable(${PROJECT_NAME} WIN32 ${HDRS} ${SRCS})

set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX _d)
target_link_libraries(
  ${PROJECT_NAME} 
  ${OGRE_LIBRARIES} 
  ${OIS_LIBRARIES} 
  # ${CEGUI_LIBRARIES}
  # ${CEGUIOGRE_LIBRARIES}
  ${PolyVox_LIBRARIES}
  AccidentalNoise
)

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/dist/bin)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/dist/media)

if ((${OGRE_VERSION} VERSION_EQUAL "1.9.0") OR (${OGRE_VERSION} VERSION_GREATER "1.9.0"))
  include_directories(${OGRE_Overlay_INCLUDE_DIR})
  target_link_libraries(${PROJECT_NAME} ${OGRE_Overlay_LIBRARIES})
endif()

# post-build copy for win32
if(WIN32 AND NOT MINGW)
  add_custom_command( TARGET ${PROJECT_NAME} PRE_BUILD
    COMMAND if not exist .\\dist\\bin mkdir .\\dist\\bin )
  add_custom_command( TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND copy \"$(TargetPath)\" .\\dist\\bin )
endif(WIN32 AND NOT MINGW)

if(MINGW OR UNIX)
	set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/dist/bin)
endif(MINGW OR UNIX)

if(WIN32)
  install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
    CONFIGURATIONS All)
  install(DIRECTORY ${CMAKE_SOURCE_DIR}/dist/Media
    DESTINATION ./
    CONFIGURATIONS Release RelWithDebInfo Debug
    )
  install(FILES ${CMAKE_SOURCE_DIR}/dist/bin/plugins.cfg
    ${CMAKE_SOURCE_DIR}/dist/bin/resources.cfg
    DESTINATION bin
    CONFIGURATIONS Release RelWithDebInfo
    )

  install(FILES ${CMAKE_SOURCE_DIR}/dist/bin/plugins_d.cfg
    ${CMAKE_SOURCE_DIR}/dist/bin/resources_d.cfg
    DESTINATION bin
    CONFIGURATIONS Debug
    )

  if(EXISTS ${OGRE_Overlay_BINARY_REL})
    install(FILES ${OGRE_Overlay_BINARY_REL} DESTINATION bin CONFIGURATIONS Release)
  endif()
  install(FILES ${OGRE_BINARY_REL}
    ${OGRE_Plugin_OctreeSceneManager_REL}
    ${OGRE_PLUGIN_DIR_REL}/cg.dll
    ${OGRE_Plugin_CgProgramManager_REL}
    ${OGRE_Plugin_ParticleFX_REL}
    ${OGRE_Terrain_BINARY_REL}
    ${OGRE_Paging_BINARY_REL}
    DESTINATION bin
    CONFIGURATIONS Release)

  if(EXISTS ${OGRE_Overlay_BINARY_DBG})
    install(FILES ${OGRE_Overlay_BINARY_DBG} DESTINATION bin CONFIGURATIONS Debug)
  endif()
  install(FILES ${OGRE_BINARY_DBG}
    ${OGRE_Plugin_OctreeSceneManager_DBG}
    ${OGRE_PLUGIN_DIR_DBG}/cg.dll
    ${OGRE_Plugin_CgProgramManager_DBG}
    ${OGRE_Plugin_ParticleFX_DBG}
    ${OGRE_Terrain_BINARY_DBG}
    ${OGRE_Paging_BINARY_DBG}
    DESTINATION bin
    CONFIGURATIONS Debug)
endif(WIN32)

if(UNIX)
  
  install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
    CONFIGURATIONS All)
  
  install(DIRECTORY ${CMAKE_SOURCE_DIR}/dist/media
    DESTINATION ./
    CONFIGURATIONS Release RelWithDebInfo Debug
    )
  
  install(FILES ${CMAKE_SOURCE_DIR}/dist/bin/plugins.cfg
    ${CMAKE_SOURCE_DIR}/dist/bin/resources.cfg
    DESTINATION bin
    CONFIGURATIONS Release RelWithDebInfo Debug
    )
  
endif(UNIX)
 
