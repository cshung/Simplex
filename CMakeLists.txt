cmake_minimum_required (VERSION 3.5)
set (CMAKE_CXX_STANDARD 14)

# Set the CMP0135 policy to NEW
if(POLICY CMP0135)
  cmake_policy(SET CMP0135 NEW)
endif()

project (simplex)
if(WIN32)
    set(CMAKE_CXX_FLAGS_RELEASE "/MT")
    set(CMAKE_CXX_FLAGS_DEBUG "/MTd")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /NODEFAULTLIB:msvcrtd")
endif()
enable_testing()
add_subdirectory(src)
add_subdirectory(test)