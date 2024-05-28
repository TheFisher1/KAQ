# Set the project name as the binary name
set(BINARY ${CMAKE_PROJECT_NAME})

# Recursively get all .h and .cpp files in the current directory and its subdirectories
file(GLOB_RECURSE SOURCES LIST_DIRECTORIES true ${CMAKE_SOURCE_DIR}/src/*.h ${CMAKE_SOURCE_DIR}/src/*.cpp)

# Ensure SOURCES is defined
set(SOURCES ${SOURCES})

# Add the executable target using the sources
add_executable(${BINARY}_run ${SOURCES})

# Add a static library target using the same sources
add_library(${BINARY}_lib STATIC ${SOURCES})

# Optionally, you can set include directories if your project needs it
target_include_directories(${BINARY}_run PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_include_directories(${BINARY}_lib PRIVATE ${CMAKE_SOURCE_DIR}/include)
