cmake_minimum_required(VERSION 3.28)
project(octocus)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_SCAN_FOR_MODULES OFF)

set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
option(BUILD_SHARED_LIBS OFF)
option(ENABLE_ICE OFF)
option(OPENSSL_USE_STATIC_LIBS ON)
option(Protobuf_USE_STATIC_LIBS ON)

add_subdirectory(raylib)
add_subdirectory(entt)
#cmake_policy(set CMP0074 NEW)
set(CMAKE_POLICY_DEFAULT_CMP0074 NEW)


find_package(GameNetworkingSockets CONFIG REQUIRED)


include_directories(components)
include_directories(systems)

include("target_files.cmake")

add_executable(octocus ${OCTOCUS_SOURCES})

target_link_libraries(octocus raylib EnTT::EnTT GameNetworkingSockets::shared)

add_executable(octocus_server ${OCTOCUS_SOURCES})
target_link_libraries(octocus_server EnTT::EnTT GameNetworkingSockets::shared)
