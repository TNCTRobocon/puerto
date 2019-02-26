cmake_minimum_required(VERSION 3.0.0)
project(puerto VERSION 0.1.0)

include(CTest)
enable_testing()
set (local ${HOME}/local)

# c++ complile options 
set(CMAKE_CXX_STANDARD 17)
enable_language(CXX)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wno-long-long -pedantic -std=c++17")


add_executable(puerto  
    main.cpp
)
include_directories(.)
add_subdirectory(boot)
add_subdirectory(utils)
add_subdirectory(net)
target_link_libraries(puerto boot utils net)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
# c++ librarys
## zeromq binded for cpp
find_package(cppzmq)
target_link_libraries(puerto cppzmq)

set(Boost_USE_STATIC_LIBS        ON)
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME     ON)
set(BOOST_ROOT ${LOCAL}/boost)
find_package(Boost 1.65.1)