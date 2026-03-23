cmake_minimum_required(VERSION 3.10)
project(OrderMatchingEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Threads REQUIRED)

include_directories(include)

file(GLOB SOURCES "src/*.cpp")

add_executable(MatchingEngine ${SOURCES})
target_link_libraries(MatchingEngine Threads::Threads)
