cmake_minimum_required(VERSION 3.10)
project(MySFMLApp)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

add_executable(app main.cpp)
target_link_libraries(app sfml-graphics sfml-window sfml-system)
