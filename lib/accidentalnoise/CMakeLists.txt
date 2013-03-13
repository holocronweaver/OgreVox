set(AccidentalNoise_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/include")
#include_directories("${CMAKE_CURRENT_SOURCE_DIR}")
include_directories(${AccidentalNoise_INCLUDE_DIRS})


file(GLOB_RECURSE AN_include include/*.h include/*.cpp)
file(GLOB AN_src src/*.h src/*.cpp)

add_library(AccidentalNoise ${AN_include} ${AN_src})


#set(AccidentalNoise_LIBRARY_DIR src)

#message(STATUS ${AN_src})