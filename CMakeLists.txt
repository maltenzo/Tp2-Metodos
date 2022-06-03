cmake_minimum_required(VERSION 3.0.0)
project(tp2 VERSION 0.1.0)

IF(CMAKE_BUILD_TYPE MATCHES Debug)
    message("Debug mode")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g3 -O0")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
ENDIF()

IF(CMAKE_BUILD_TYPE MATCHES Release)
    message("Release mode")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -fopenmp")
ENDIF()

# Prender y apagar este flag para soporte OpenMP
set(USE_OpenMP ON)
# Script cmake ``multi-plataforma`` (testeado en MacOS y Ubuntu)
include(cmake/OpenMP.cmake)

include(CTest)
enable_testing()

add_executable(tp2 main.cpp)

# Por cada módulo que use OpenMP ponerlo como dependencia de esta forma
target_link_libraries(tp2 LINK_PUBLIC OpenMP::OpenMP_CXX)


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
set(CMAKE_BUILD_TYPE Debug)
include(CPack)
