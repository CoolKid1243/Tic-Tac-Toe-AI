cmake_minimum_required(VERSION 3.24)
project(TicTacToe-AI)

# Find libraries
#find_package('package name' REQUIRED)

# Add all .cpp files in the src directory and its subdirectories
file(GLOB_RECURSE CPP_SOURCES src/*.cpp)

# Add all .h files in the src directory and its subdirectories
file(GLOB_RECURSE HEADER_FILES src/*.h)

# Create the library target
add_executable(${PROJECT_NAME} src/main.cpp ${CPP_SOURCES} ${HEADER_FILES})

# Link libraries to the target
#target_link_libraries(${PROJECT_NAME} 'package name')
