# AGENTS.md — Enix Game Engine

Guidance for AI coding agents working in this repo. Mirrors `CLAUDE.md`; keep the two in sync.

## Project Overview

Enix is a Vulkan-based game engine in C++20. Learning/research project for engine architecture, Vulkan, and modern C++. All engine code lives in the `Enix` namespace.

## Build

```bash
cmake -S . -B <build_dir>
cmake --build <build_dir>
```

Build configurations:
- `cmake-build-debug` — CLion default
- `cmake-build-debug-mingw` — MinGW/MSYS2
- `cmake-build-debug-visual-studio` — MSVC

Shaders compile as a CMake custom target (`Shaders`) via `glslc`; `Enix` depends on it. SPV files are emitted next to the GLSL source as `<name>.<ext>.spv`. Binaries land in `bin/`; runtime CWD is `bin/`, so assets resolve via `../` (`_workspaceRoot = "../"`).

## Architecture

### Entry point
- `Enix/Main.cpp` — constructs `VulkanEngine`, calls `run()`.

### Engine
- `Enix/Engine.h` — abstract base (`tick`, `drawUI`, `run`).
- `Enix/VulkanEngine.{h,cpp}` — singleton-ish engine; owns `Window`, `InputSystem`, `Renderer`, `AppUI`, `ThreadPool`, `Scene`. `getInstance()` is valid only after `Main.cpp` constructs the singleton.

### Subsystems (under `Enix/`)

| Directory | Purpose |
|-----------|---------|
| `Render/` | Vulkan wrapper — `Instance`, `Device`, `WindowSurface`, `SwapChain`, `RenderPass`, `GraphicsPipeline`, `Renderer`, `Texture`, `Model`, `Material` |
| `Core/`   | Engine objects — `Actor`, `MeshActor`, `Scene`, `Camera`, `Transform`, `PointLight`, `ThreadPool` |
| `Asset/`  | Asset loaders — `MeshAsset` (tinyobjloader), `TextureAsset` (stb_image) |
| `Input/`  | `InputSystem` — GLFW keyboard input |
| `UI/`     | `AppUI` — ImGui inspector (camera, point light, actors, FPS) |

### Render pipeline

Single main render pass; ImGui inspector renders inline at the end of that pass — no separate ImGui render pass or command buffer:
1. Scene draw — Blinn-Phong via `GraphicsPipeline`; one draw per `MeshActor` with per-actor descriptor set + push constants.
2. `ImGui_ImplVulkan_RenderDrawData` recorded into the same primary command buffer just before `vkCmdEndRenderPass`.

`Renderer::draw()` flow each frame:
1. `ImGui_ImplVulkan_NewFrame()` / `ImGui_ImplGlfw_NewFrame()` / `ImGui::NewFrame()`
2. `VulkanEngine::drawUI()` → `AppUI::onDraw()` (builds the dockspace, menu bar, and the **Inspector** window)
3. `ImGui::Render()`
4. `updateCamera()` + `drawFrame()` (submit + present)
5. Optional `ImGui::UpdatePlatformWindows()` / `ImGui::RenderPlatformWindowsDefault()` (off by default; see below)

ImGui init binds to `_renderPass.renderPass()` (the main pass) and uses `_device.graphicsQueue()`. Fonts are uploaded via a one-shot command buffer.

Multi-viewport (`ImGuiConfigFlags_ViewportsEnable`) is intentionally disabled — enabling it requires maintaining ImGui-managed per-platform-window swapchains across resizes, which we don't yet do. Re-enable when that lands.

Double-buffered with `_maxFramesInFlight = 2`. UBO: view, proj, cameraPosition, lightPosition, lightColor.

### Sync correctness

`vkResetFences` runs **after** `vkAcquireNextImageKHR` returns a code that will lead to a queue submit. Resetting before the swapchain-out-of-date early return would leave the fence unsignaled and deadlock the next `vkWaitForFences`.

## Third-Party (`ThirdParty/`)

| Library | Use |
|---------|-----|
| `glfw` | Window + input |
| `glm`  | Math (`GLM_FORCE_DEPTH_ZERO_TO_ONE`) |
| `imgui` | Debug/editor UI (docking branch, viewports off) |
| `spdlog` | Logging |
| `stb_image` | Texture loading |
| `tinyobjloader` | OBJ mesh loading |

## Code Conventions

- C++20; no platform-specific extensions.
- `#pragma once` on every header.
- Member variables prefixed with `_` (e.g., `_window`, `_renderer`).
- Forward-declare `VulkanEngine` where possible.
- Vulkan handles are owned by their enclosing class; release in `cleanUp()` / destructor.
- Render objects are non-copyable — delete copy ctor and copy-assign.

## Asset Paths

Resolved relative to workspace root (`../` from `bin/`): `Models/`, `Textures/`, `Shaders/`.

## Planned Features (Todos)

Mouse input, Skybox, Serialization, PBR, Threaded rendering, Asset manager, Networking, Scene graph, Physics, Animation, Audio, Task system, ImGui multi-viewport.
