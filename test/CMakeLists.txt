set(BINARY ${CMAKE_PROJECT_NAME})

file(GLOB_RECURSE TEST_SOURCES LIST_DIRECTORIES true ${CMAKE_SOURCE_DIR}/test/*.h ${CMAKE_SOURCE_DIR}/test/*.cpp)

add_executable(${BINARY}_test ${TEST_SOURCES})

target_link_libraries(${BINARY}_test ${BINARY}_lib)

target_include_directories(${BINARY}_test PRIVATE ${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/test)

include(CTest)
enable_testing()
add_test(NAME ${BINARY}_test COMMAND ${BINARY}_test)
