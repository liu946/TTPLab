cmake_minimum_required(VERSION 3.6)
project(TTPLab)

set(CMAKE_CXX_FLAGS "-Wall -std=c++11")

# look for Boost
if(DEFINED ENV{BOOST_ROOT})
    set(Boost_NO_SYSTEM_PATHS ON)
endif()
set(Boost_REALPATH ON)
find_package(Boost COMPONENTS program_options serialization REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
set(LIBS ${LIBS} ${Boost_LIBRARIES})

add_subdirectory(include)
include_directories(include)
set(LIBS ${LIBS} base_static_lib)

include_directories(~/soft/eigen) # -I
include_directories(~/soft/dynet) # -I
link_directories(~/soft/dynet/build/dynet) # -L
add_subdirectory(src)
