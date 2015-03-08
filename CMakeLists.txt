cmake_minimum_required(VERSION 2.8)

set(APPLICATION_NAME "OgreVox")

project(${APPLICATION_NAME})

add_subdirectory(BareOgre)
add_subdirectory(lib)

if (WIN32)
  # set(OGRE_STATIC TRUE)
  set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++11")
  set(CMAKE_MODULE_PATH "$ENV{OGRE_HOME}/CMake/;${CMAKE_MODULE_PATH}")
  set(OGRE_SAMPLES_INCLUDEPATH "$ENV{OGRE_HOME}/Samples/include")
endif(WIN32)

if (UNIX)
  set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++11")
  if (EXISTS "/usr/local/lib/OGRE/cmake")
    set(CMAKE_MODULE_PATH "/usr/local/lib/OGRE/cmake/;${CMAKE_MODULE_PATH}")
    set(OGRE_SAMPLES_INCLUDEPATH "/usr/local/share/OGRE/samples/Common/include/")
  elseif (EXISTS "/usr/lib/OGRE/cmake")
    set(CMAKE_MODULE_PATH "/usr/lib/OGRE/cmake/;${CMAKE_MODULE_PATH}")
    set(OGRE_SAMPLES_INCLUDEPATH "/usr/share/OGRE/samples/Common/include/")
  else ()
    message(SEND_ERROR "Failed to find module path.")
  endif ()
endif()

if (CMAKE_BUILD_TYPE STREQUAL "")
  # CMake defaults to leaving CMAKE_BUILD_TYPE empty. This screws up
  # differentiation between debug and release builds.
  set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING
    "Choose the type of build, options are: None (CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif ()

set(CMAKE_DEBUG_POSTFIX "_d")
set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_BINARY_DIR}/dist")

find_package(OGRE REQUIRED)

find_package(SDL2 REQUIRED)

# find_package(Bullet REQUIRED)

if (NOT SDL2_FOUND)
  message(SEND_ERROR "Failed to find SDL2.")
endif ()

# find_package(CEGUI REQUIRED)

# if (NOT CEGUI_FOUND)
#   message(SEND_ERROR "Failed to find CEGUI.")
# endif ()

# find_package(CEGUI_OgreRenderer REQUIRED)
# find_package(CEGUI_OGRE REQUIRED)

# if (NOT CEGUI_OgreRenderer_FOUND)
#   message(SEND_ERROR "Failed to find CEGUI_OGRE.")
# endif ()

# find_package(PolyVox REQUIRED)

# if(NOT PolyVox_FOUND)
#   message(SEND_ERROR "Failed to find PolyVox.")
# endif()

file(GLOB HDRS "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
file(GLOB SRCS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/BareOgre
  ${OGRE_INCLUDE_DIRS}
  ${SDL2_INCLUDE_DIRS}
  ${BULLET_INCLUDE_DIRS}
  # ${CEGUI_INCLUDE_DIRS}
  # ${CEGUI_OGRE_INCLUDE_DIRS}
  # ${CEGUI_OgreRenderer_INCLUDE_DIRS}
  # ${PolyVox_INCLUDE_DIRS}
  "${PROJECT_SOURCE_DIR}/lib/polyvox/include"
  "${PROJECT_SOURCE_DIR}/lib/accidentalnoise/include"
  #${AccidentalNoise_INCLUDE_DIRS}
  )

add_executable(${PROJECT_NAME} WIN32 ${HDRS} ${SRCS} ${PolyVox_HDRS} ${PolyVox_SRCS})

set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX _d)

target_link_libraries(
  ${PROJECT_NAME}
  BareOgre
  ${OGRE_LIBRARIES}
  ${SDL2_LIBRARIES}
  # ${BULLET_LIBRARIES}
  # ${CEGUI_LIBRARIES}
  # ${CEGUI_OGRE_LIBRARIES}
  # ${CEGUI_OgreRenderer_LIBRARIES}
  # ${PolyVox_LIBRARIES}
  AccidentalNoise
  # PolyVox
  pthread
  )

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/dist/bin)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/dist/media)

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
