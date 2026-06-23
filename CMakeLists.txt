cmake_minimum_required(VERSION 3.20)
project(3d-scene LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# vcpkg provides these via CMAKE_TOOLCHAIN_FILE — see README setup step
find_package(glfw3  CONFIG REQUIRED)
find_package(glad   CONFIG REQUIRED)
find_package(glm    CONFIG REQUIRED)

add_executable(renderer
    src/main.cpp
    src/Shader.cpp
    src/Mesh.cpp
    src/ObjLoader.cpp
)

target_link_libraries(renderer PRIVATE glfw glad::glad glm::glm)

# Copy shaders and assets next to the exe after every build
add_custom_command(TARGET renderer POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/shaders  $<TARGET_FILE_DIR:renderer>/shaders
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/assets   $<TARGET_FILE_DIR:renderer>/assets
    COMMENT "Copying shaders and assets to output directory"
)
