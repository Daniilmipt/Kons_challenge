cmake_minimum_required(VERSION 3.26)
project(Kons_challenge)
find_package(
        Boost 1.65.1 REQUIRED
        COMPONENTS  json
)

set(CMAKE_CXX_STANDARD 20)
set(Boost_DEBUG 1)
SET(Boost_USE_STATIC_LIBS ON)

find_package(Boost COMPONENTS thread log log_setup REQUIRED)

message(STATUS Boost_LIBRARIES:)
message (STATUS ${Boost_LIBRARIES})
message(STATUS BOOST_INCLUDEDIR:)
message(STATUS ${BOOST_INCLUDEDIR})

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    link_directories(${Boost_LIBRARY_DIRS})
endif()

add_executable(Kons_challenge main.cpp
)
target_link_libraries(Kons_challenge pthread Boost::log Boost::log_setup )
target_link_libraries(Kons_challenge boost_system boost_iostreams )