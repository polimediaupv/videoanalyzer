#Specify the version being used aswell as the language
cmake_minimum_required(VERSION 2.6)
#Name your project here
project(VideoAnalyzer)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake-modules)
Message(STATUS ${CMAKE_CURRENT_SOURCE_DIR})
Message(STATUS ${CMAKE_MODULE_PATH})

#set(OpenCV_DIR "/opt/local/lib/cmake/" CACHE PATH "Path to OpenCV build directory - should include OpenCVConfig.cmake.")
FIND_PACKAGE( OpenCV REQUIRED )

SET(EXECUTABLE_OUTPUT_PATH "${PROJECT_BINARY_DIR}/bin")

IF(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  SET(CMAKE_INSTALL_PREFIX
    "${PROJECT_SOURCE_DIR}" CACHE PATH "install prefix" FORCE
    )
ENDIF(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)

set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE TYPE INTERNAL FORCE )

Message(STATUS ${CMAKE_CURRENT_BINARY_DIR})
Message(STATUS ${PROJECT_BINARY_DIR})
Message(STATUS ${EXECUTABLE_OUTPUT_PATH})

include_directories("${VideoAnalyzer_SOURCE_DIR}/include" ${OpenCV_INCLUDE_DIRS})
file(GLOB_RECURSE VideoAnalyzer_SOURCES "src/*.cpp")

add_executable(videoanalyzer main/main.cpp ${VideoAnalyzer_SOURCES})

TARGET_LINK_LIBRARIES( videoanalyzer ${OpenCV_LIBS} )

install (TARGETS videoanalyzer DESTINATION "${PROJECT_SOURCE_DIR}/bin")
