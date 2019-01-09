cmake_minimum_required(VERSION 3.12)
project(Final_Clion C)

set(CMAKE_C_STANDARD 99)

add_executable(Final_Clion main.c general.h bullet.h bullet.c character.h character.c levelSetting.h levelSetting.c fireworks.c fireworks.h)
include_directories(D:/Allegro/allegro-5.0.10-mingw-4.7.0/include)
if (CMAKE_BUILD_TYPE MATCHES Debug)
    add_definitions(-DDEBUG)
    target_link_libraries(Final_Clion D:/Allegro/allegro-5.0.10-mingw-4.7.0/lib/liballegro-5.0.10-monolith-mt-debug.a)
endif()

if (CMAKE_BUILD_TYPE MATCHES Release)
    target_link_libraries(Final_Clion D:/Allegro/allegro-5.0.10-mingw-4.7.0/lib/liballegro-5.0.10-monolith-mt.a)
endif ()