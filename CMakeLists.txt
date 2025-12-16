cmake_minimum_required(VERSION 3.20)
project(httpserver)

set(CMAKE_CXX_STANDARD 17) #C++ 17
set(CMAKE_CXX_STANDARD_REQUIRED ON )


add_executable(httpserver
  src/http-server.cpp
)

target_include_directories(httpserver PRIVATE include)
