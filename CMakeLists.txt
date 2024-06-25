cmake_minimum_required(VERSION 3.28)
project(EntityTest)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples

add_subdirectory(raylib)
add_subdirectory(entt)

include_directories(components)
include_directories(systems)

add_executable(${PROJECT_NAME} main.cpp
        components/SpaceComponents.h
        components/LifeComponents.h
        components/RenderComponent.h
        systems/DamageSystem.cpp
        systems/DamageSystem.h
        systems/InputSystem.h
        systems/InputSystem.cpp
        systems/DrawSystem.h
        systems/DrawSystem.cpp
        systems/MovementSystem.h
        systems/MovementSystem.cpp
        game.h game.cpp)

target_link_libraries(${PROJECT_NAME} raylib EnTT::EnTT "-static-libgcc -static-libstdc++")
