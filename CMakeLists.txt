cmake_minimum_required(VERSION 3.16)
project(mario)

set(CMAKE_CXX_STANDARD 17)

cmake_policy(SET CMP0135 NEW)

#include(FetchContent)
#set(FETCHCONTENT_QUIET FALSE)
#
#FetchContent_Declare(
#    ptsd
#
#    URL         https://github.com/ntut-open-source-club/practical-tools-for-simple-design/archive/refs/tags/v0.2.zip
#
#    # Or alternatively if you want to use a specific git commit instead of a release:
#    # GIT_REPOSITORY  https://github.com/ntut-open-source-club/practical-tools-for-simple-design.git
#    # GIT_TAG         <git_tag>
#
#    SOURCE_DIR  ${CMAKE_CURRENT_SOURCE_DIR}/PTSD
#)
#
#FetchContent_MakeAvailable(ptsd)
add_subdirectory(PTSD)

include(files.cmake)
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
list(TRANSFORM SRC_FILES PREPEND ${SRC_DIR}/)
list(TRANSFORM INCLUDE_FILES PREPEND ${INCLUDE_DIR}/)

# file(GLOB_RECURSE SRC_FILES src/*)
# file(GLOB_RECURSE HEADER_FILES include/*)

add_executable(${PROJECT_NAME}
    ${SRC_DIR}/main.cpp
    ${SRC_FILES}
    ${HEADER_FILES}
        include/Mario.hpp
        src/Mario.cpp
        include/Character.hpp
        src/Character.cpp
        include/Tile.hpp
        include/LevelManager.hpp
        src/LevelManager.cpp
        include/QuestionBlock.hpp
        src/Tile.cpp
        include/EffectManager.hpp
        include/Brick.hpp
        src/Brick.cpp
        include/Definitions.hpp
        src/EffectManager.cpp
        src/QuestionBlock.cpp
        include/Item.hpp
        include/ItemManager.hpp
        src/ItemManager.cpp
        src/Mushroom.cpp
        src/Starman.cpp
        src/Coin.cpp
        include/Enemy.hpp
        src/Enemy.cpp
        include/Goomba.hpp
        src/Goomba.cpp
        include/Koopa.hpp
        src/Koopa.cpp
        include/GameHUD.hpp
        src/GameHUD.cpp
        src/Starman.cpp
        src/Fireball.cpp
        src/FireFlower.cpp
        include/Lift.hpp
        src/Lift.cpp
        include/PiranhaPlant.hpp
        src/PiranhaPlant.cpp
        include/SFXManager.hpp
        src/SFXManager.cpp
)

if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(${PROJECT_NAME} PRIVATE RESOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}/Resources")
else()
    message(FATAL_ERROR "relative RESOURCE_DIR is WIP, Please use `-DCMAKE_BUILD_TYPE=Debug` build for now.")
    # target_compile_definitions(${PROJECT_NAME} PRIVATE RESOURCE_DIR="${CMAKE_INSTALL_PREFIX}/Resources")
endif()

target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE ${DEPENDENCY_INCLUDE_DIRS})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/PTSD/include)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(${PROJECT_NAME}
    SDL2::SDL2main
    PTSD
)
