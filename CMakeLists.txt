cmake_minimum_required(VERSION 3.0.0)
project(calc VERSION 0.1.0)

include(CTest)
enable_testing()

add_executable(calc main.cpp calc.h)


IF (WIN32)
add_definitions(-D_MSVC)
ENDIF()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
