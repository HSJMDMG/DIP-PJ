cmake_minimum_required(VERSION 3.7)
project(BayesianRemoval)
find_package(OpenCV)
include_directories( ${OpenCV_INCLUDE_DIRS} )
set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp
        src/BayesianRefine.h
        src/BayesianRefine.cpp
        src/RoughRemoval.h
        src/RoughRemoval.cpp
        src/IntensityDetector.cpp src/IntensityDetector.h src/HarmonizationRefine.cpp src/HarmonizationRefine.h)
add_executable(BayesianRemoval ${SOURCE_FILES} )
target_link_libraries(BayesianRemoval ${OpenCV_LIBS} )