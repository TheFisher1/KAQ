set(BINARY ${CMAKE_PROJECT_NAME})

file(GLOB_RECURSE SOURCES LIST_DIRECTORIES true ${CMAKE_SOURCE_DIR}/src/*.h ${CMAKE_SOURCE_DIR}/src/*.cpp)

set(SOURCES ${SOURCES})

add_executable(${BINARY}_run ${SOURCES})

add_library(${BINARY}_lib STATIC ${SOURCES})

target_include_directories(${BINARY}_run PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_include_directories(${BINARY}_lib PRIVATE ${CMAKE_SOURCE_DIR}/include)
