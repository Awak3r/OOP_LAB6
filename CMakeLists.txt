
cmake_minimum_required(VERSION 3.10)
project(task6)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)



set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror=maybe-uninitialized")


include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.15.0
  TLS_VERIFY false
)

include_directories(${PROJECT_SOURCE_DIR}/include)
add_library(${CMAKE_PROJECT_NAME}_lib INTERFACE)
target_include_directories(${CMAKE_PROJECT_NAME}_lib INTERFACE ${PROJECT_SOURCE_DIR}/include)

add_executable(${CMAKE_PROJECT_NAME}_exe main.cpp)
target_link_libraries(${CMAKE_PROJECT_NAME}_exe PRIVATE ${CMAKE_PROJECT_NAME}_lib)


enable_testing()

add_executable(tests test/tests01.cpp)
target_link_libraries(tests ${CMAKE_PROJECT_NAME}_lib gtest_main gtest)

add_test(NAME GrowCounterTests COMMAND tests)