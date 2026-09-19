cmake_minimum_required(VERSION 3.22.1)
project("parkurgame")

add_library(parkurgame SHARED
    native-lib.cpp
    Shader.cpp
    Mesh.cpp
    ParkourGenerator.cpp
    Player.cpp
    GameRenderer.cpp
)

find_library(log-lib log)
find_library(GLESv3-lib GLESv3)
find_library(android-lib android)

target_link_libraries(parkurgame
    ${log-lib}
    ${GLESv3-lib}
    ${android-lib})
