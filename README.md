# 3d-scene

An OpenGL 3.3 renderer written in C++17. Loads `.obj` meshes, runs a Phong lighting model through a hand-written GLSL pipeline, and handles windowing and input via GLFW.

Built this to get hands-on with the full graphics pipeline from scratch — no engine, no abstraction library.

## What it does

- GLFW window with a resizable OpenGL 3.3 Core context
- VAO/VBO/EBO mesh upload with indexed rendering
- `.obj` parser with fan triangulation for quads/polygons and flat-normal fallback for meshes without `vn` data
- Phong shading (ambient, diffuse, specular) in GLSL
- MVP transform via GLM — model rotates over time, camera is fixed

## Dependencies

Managed through vcpkg:

| Library | Purpose |
|---------|---------|
| glfw3   | Window and input |
| glad    | OpenGL function loader |
| glm     | Math (vectors, matrices) |

## Building

Requires CMake 3.20+, Visual Studio 2022, and vcpkg at `C:\vcpkg`.

```powershell
# First time only
cmake -B build -S . `
  -DCMAKE_TOOLCHAIN_FILE="C:/vcpkg/scripts/buildsystems/vcpkg.cmake" `
  -DVCPKG_TARGET_TRIPLET=x64-windows

# Every build after that
cmake --build build --config Release
```

Shaders and assets are copied next to the `.exe` automatically after each build.

```powershell
.\build\Release\renderer.exe
```

Press **Esc** to exit.

## Project structure

```
src/
  main.cpp          window setup, render loop, scene state
  Shader.h/.cpp     GLSL load, compile, link, uniform helpers
  Mesh.h/.cpp       GPU buffer upload and draw call
  ObjLoader.h/.cpp  .obj parser → Mesh
  Camera.h          view and projection matrices
shaders/
  phong.vert        MVP transform, world-space interpolants
  phong.frag        Phong lighting
assets/
  cube.obj          default test mesh
```

## Swapping in your own model

Replace `assets/cube.obj` with any `.obj` file that has triangles or quads, or change the path passed to `loadObj` in `main.cpp`. Normals are generated automatically if the file doesn't include them.
