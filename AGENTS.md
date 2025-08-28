# Repository Guidelines

## Project Structure & Module Organization
- `Enix/`: C++20 source (engine entry in `Main.cpp`). Submodules: `Render/`, `Core/`, `Asset/`, `Input/`, `UI/`.
- `Shaders/`: GLSL shaders (`.vert`, `.frag`) compiled to SPIR-V (`.spv`).
- `Models/`, `Textures/`: runtime assets loaded by the engine.
- `ThirdParty/`: vendored libs (GLFW, GLM, ImGui, stb_image, tinyobjloader, spdlog).
- `bin/`: build output directory (runtime working dir).

## Build, Test, and Development Commands
- Configure: `cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug`
- Build app: `cmake --build build --config Debug`
- Build shaders only: `cmake --build build --target Shaders`
- Run (Windows): `bin\Enix.exe`  Run (Unix): `./bin/Enix`
Notes: Requires Vulkan SDK; ensure `glslc` is on PATH (`%VULKAN_SDK%\Bin`). CLion users can open the root CMake project; `Shaders` target runs before `Enix`.

## Coding Style & Naming Conventions
- Language: C++20. Indent with 4 spaces; braces on same line.
- Files: PascalCase per type (e.g., `Renderer.h/.cpp`). Headers use `#pragma once`.
- Types: PascalCase (`Device`, `Renderer`). Methods/functions: camelCase. Members: `_camelCase`.
- Namespaces: use `Enix` where appropriate.
- Logging: use `spdlog` (`spdlog::debug/info/error`); prefer early returns and RAII.

## Testing Guidelines
- Current state: no dedicated unit test suite.
- Additions: place tests under `tests/` using Catch2 or GoogleTest, and wire via CTest.
- Naming: `*.test.cpp` with test names mirroring module names.
- Smoke tests: verify engine launches and renders a frame without validation errors (enable Vulkan validation layers).

## Commit & Pull Request Guidelines
- Commits: imperative, concise, scoped messages (e.g., "Add ImGui support", "Fix normal matrix"). Group related changes.
- PRs: clear description, linked issues, practical test notes, and screenshots for rendering/UI changes (`Screenshots/`). Keep diffs focused.

## Security & Configuration Tips
- Vulkan SDK: set `VULKAN_SDK` and include `%VULKAN_SDK%\Bin` in PATH so `glslc` is found by CMake.
- Drivers: update GPU drivers; enable validation layers in debug builds.
- Assets: keep large binaries out of Git where possible; prefer `ThirdParty/` for vendored source.

