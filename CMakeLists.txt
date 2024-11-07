cmake_minimum_required(VERSION 3.10)
project(CameraProcessorApp)

# Set C++ Standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find OpenCV
find_package(OpenCV REQUIRED)

# Add executable
add_executable(CameraProcessorApp main.cc CameraProcessor.cc)

# Include directories
target_include_directories(CameraProcessorApp PRIVATE ${OpenCV_INCLUDE_DIRS})

# Link OpenCV libraries
target_link_libraries(CameraProcessorApp PRIVATE ${OpenCV_LIBS})
