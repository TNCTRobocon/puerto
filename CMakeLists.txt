cmake_minimum_required(VERSION 3.0.0)
project(Puerto VERSION 0.1.0)

include(CTest)
enable_testing()

# c++ complile options 
set(CMAKE_CXX_STANDARD 17)
enable_language(CXX)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wno-long-long -pedantic")


add_executable(Puerto  main.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
# c++ librarys
## zeromq binded for cpp
find_package(cppzmq)
target_link_libraries(Puerto cppzmq)

#set(Boost_USE_STATIC_LIBS        ON)
#set(Boost_USE_MULTITHREADED      ON)
#set(Boost_USE_STATIC_RUNTIME     ON)
#find_package(Boost 1.36.0 REQUIRED COMPONENTS date_time system)