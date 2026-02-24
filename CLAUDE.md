# CLAUDE.md — Enix Game Engine

## Project Overview

Enix is a Vulkan-based game engine written in C++20. It is a learning/research project for studying game engine architecture, Vulkan, and modern C++.

## Build

```bash
cmake -S . -B <build_dir>
cmake --build <build_dir>
```

Build configurations in use:
- `cmake-build-debug` — CLion debug (default)
- `cmake-build-debug-mingw` — MinGW/MSYS2
- `cmake-build-debug-visual-studio` — MSVC

Shaders are compiled automatically via `glslc` as a CMake custom target (`Shaders`). The `Enix` executable target depends on `Shaders`.

Compiled binaries go to `bin/`. When launched via the provided CMake/Visual Studio settings, the working directory at runtime is `bin/`, and assets are resolved relative to the workspace root using the `../` prefix (see `_workspaceRoot = "../"` in `VulkanEngine.h`).

## Shader Compilation

GLSL shaders live in `Shaders/` (`.vert`, `.frag`, `.comp`). SPV output is written alongside the source file as `<name>.<ext>.spv`. Recompile by rebuilding the `Shaders` CMake target.

## Architecture

### Entry point
- `Enix/Main.cpp` — creates `VulkanEngine`, calls `run()`

### Engine
- `Enix/Engine.h` — abstract base (`tick`, `drawUI`, `run`)
- `Enix/VulkanEngine.h/.cpp` — singleton engine; owns `Window`, `InputSystem`, `Renderer`, `AppUI`, `ThreadPool`, `Scene`

### Subsystems (all under `Enix/`)

| Directory | Purpose |
|-----------|---------|
| `Render/` | Vulkan wrapper: `Instance`, `Device`, `WindowSurface`, `SwapChain`, `RenderPass`, `GraphicsPipeline`, `Renderer`, `Texture`, `Model`, `Material` |
| `Core/` | Engine objects: `Actor`, `MeshActor`, `Scene`, `Camera`, `Transform`, `PointLight`, `ThreadPool` |
| `Asset/` | Asset loading: `MeshAsset` (tinyobjloader), `TextureAsset` (stb_image) |
| `Input/` | `InputSystem` — GLFW keyboard input |
| `UI/` | `AppUI` — ImGui integration |

### Render pipeline
`Renderer` currently drives a single main render pass, with an additional ImGui pass defined but disabled/WIP:
1. Main pass — Blinn-Phong shading via `GraphicsPipeline`
2. ImGui pass (present but not currently recorded/submitted in `Renderer::drawFrame()`) — `_imguiRenderPass` with `_imguiCommandBuffers`

Double-buffering with `_maxFramesInFlight = 2`. UBO contains: view/proj matrices, camera position, light position, light color.

### Namespace
All engine code lives in the `Enix` namespace.

## Third-Party Libraries (`ThirdParty/`)

| Library | Use |
|---------|-----|
| `glfw` | Window and input |
| `glm` | Math (`GLM_FORCE_DEPTH_ZERO_TO_ONE`) |
| `imgui` | Debug/editor UI |
| `spdlog` | Logging |
| `stb_image` | Texture loading |
| `tinyobjloader` | OBJ mesh loading |

## Code Conventions

- C++20; avoid platform-specific extensions
- `#pragma once` for all headers
- Member variables prefixed with `_` (e.g., `_window`, `_renderer`)
- Forward-declare `VulkanEngine` where possible to reduce include overhead
- Resource handles (Vulkan objects) are owned by their enclosing class; clean up in `cleanUp()` / destructor
- `VulkanEngine` behaves as a singleton, but is constructed explicitly in `Enix/Main.cpp`; `VulkanEngine::getInstance()` is only valid after that construction has occurred
- Non-copyable render objects: delete copy constructor and copy-assignment operator

## Asset Paths

Assets are resolved relative to the workspace root (`../` from `bin/`):
- Models: `Models/`
- Textures: `Textures/`
- Shaders: `Shaders/`

## Planned Features (Todos)

Mouse input, Skybox, Serialization, PBR, Threaded rendering, Asset manager, Networking, Scene graph, Physics, Animation, Audio, Task system.
