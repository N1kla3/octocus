cmake_minimum_required(VERSION 3.28)
project(EntityTest)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples

add_subdirectory(raylib)
add_subdirectory(entt)

add_executable(${PROJECT_NAME} main.cpp
        SpaceComponents.h
        LifeComponents.h
        DamageSystem.cpp
        DamageSystem.h)

target_link_libraries(${PROJECT_NAME} raylib EnTT::EnTT "-static-libgcc -static-libstdc++")