cmake_minimum_required(VERSION 3.18.1)
project("ultimate3dmodeling")

set(CMAKE_CXX_STANDARD 17)

add_library(
    native-lib
    SHARED
    native-lib.cpp
    VertexData.cpp
    CubeGenerator.cpp
    Camera.cpp
)

find_library(log-lib log)

target_link_libraries(
    native-lib
    GLESv3
    EGL
    ${log-lib}
)