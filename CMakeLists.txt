#Specify the version being used aswell as the language
cmake_minimum_required(VERSION 2.6)
#Name your project here
project(VideoAnalizer)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake-modules)
Message(STATUS ${CMAKE_CURRENT_SOURCE_DIR})
Message(STATUS ${CMAKE_MODULE_PATH})

#set(OpenCV_DIR "/opt/local/lib/cmake/" CACHE PATH "Path to OpenCV build directory - should include OpenCVConfig.cmake.")
FIND_PACKAGE( OpenCV REQUIRED )

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
Message(STATUS ${CMAKE_CURRENT_BINARY_DIR})
Message(STATUS ${PROJECT_BINARY_DIR})
Message(STATUS ${EXECUTABLE_OUTPUT_PATH})

include_directories("${VideoAnalizer_SOURCE_DIR}/include" ${OpenCV_INCLUDE_DIRS})
file(GLOB_RECURSE VideoAnalizer_SOURCES "src/*.cpp")

#This tells CMake to fib.c and name it fibonacci
add_executable(videoanalizer main/main.cpp ${VideoAnalizer_SOURCES})

TARGET_LINK_LIBRARIES( videoanalizer ${OpenCV_LIBS} )

