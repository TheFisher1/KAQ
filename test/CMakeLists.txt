# Set the project name as the binary name
set(BINARY ${CMAKE_PROJECT_NAME})

# Set the test sources
file(GLOB_RECURSE TEST_SOURCES LIST_DIRECTORIES true ${CMAKE_SOURCE_DIR}/test/*.h ${CMAKE_SOURCE_DIR}/test/*.cpp)

# Add the test executable
add_executable(${BINARY}_test ${TEST_SOURCES})

# Link the test executable with the project's static library
target_link_libraries(${BINARY}_test ${BINARY}_lib)

# Include directories for the test executable
target_include_directories(${BINARY}_test PRIVATE ${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/test)

# Add the tests to CTest
include(CTest)
enable_testing()
add_test(NAME ${BINARY}_test COMMAND ${BINARY}_test)