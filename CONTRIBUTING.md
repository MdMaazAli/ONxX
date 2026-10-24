# Contributing to xONxX

Thank you for your interest in contributing to **xONxX** (*eXtensive Open Noumenal eXploration & eXperimentation*)!

xONxX is a high-performance C++ simulation and reinforcement learning game engine combining generational ECS slot architecture, live Lua scripting, ZeroMQ socket pipelines, and retro hardware tooling.

---

## 1. Guiding Philosophy

- **Noumenal Clarity**: The engine separates the underlying computational reality (noumenal state: ECS strides, physics vectors, tensor weights) from the visual presentation (phenomenal projection: ImGui, OpenGL rendering).
- **Kaizen (改善)**: We believe in continuous, thoughtful refinement. Every commit should improve stability, performance, or developer clarity.
- **Cache-Conscious Performance**: Data structures are designed for sequential memory access, cache locality, and $O(1)$ generational slot lookups without array shifting.

---

## 2. Inbound / Outbound Licensing

By contributing to **xONxX**, you agree that your contributions will be licensed under the **Apache License, Version 2.0**.

Under Section 5 of the Apache 2.0 License:
> *Unless You explicitly state otherwise, any Contribution intentionally submitted for inclusion in the Work by You to the Licensor shall be under the terms and conditions of this License, without any additional terms or conditions.*

---

## 3. How to Contribute

### Reporting Bugs
1. Check existing issues to see if the bug has already been reported.
2. Open a new issue with a clear title and description.
3. Include reproduction steps, your operating system, and terminal/log output if applicable.

### Suggesting Features & Enhancements
1. Open a discussion or feature request detailing the use case.
2. Explain how the proposed feature fits into xONxX's ECS or reinforcement learning architecture.

### Submitting Pull Requests
1. Fork the repository and create your branch from `main`:
   ```bash
   git checkout -b feature/your-feature-name
   ```
2. Follow our coding guidelines below.
3. Ensure the project compiles cleanly with **zero warnings/errors**:
   ```bash
   # From root directory:
   # Execute build task via VS Code (tasks.json) or run g++ directly
   ```
4. Submit a Pull Request describing your changes, motivation, and test results.

---

## 4. Coding Standards & Conventions

1. **Maintain Architectural Integrity**:
   - **Hot Domain**: `TransformComponent`, `PhysicsComponent` must remain pure Plain Old Data (POD) structs suitable for contiguous memory strides.
   - **Cold Domain**: Authoring metadata (`EntityMetadata`, string names, UUIDs) belongs in cold storage.
   - **Generational Safety**: Never manipulate raw array indices directly when referencing entities across frames—always use generational `SlotKey` or `UUID`.

2. **Preserve Documentation Integrity**:
   - Do not delete or overwrite architectural docstrings (such as the ECS stride comments in `Physics.h`).
   - Keep your comments clean, standard, and focused on technical rationale.

3. **Memory & Resource Safety**:
   - Always pair resource allocation with cleanup.
   - When entities are removed, ensure associated scripts, textures, or buffers are freed (e.g., calling `luaRunner.UnloadScript` before recycling slots).

---

## 5. Building the Project

xONxX is designed to build with modern C++ compilers (MinGW `g++` on Windows, GCC/Clang on POSIX systems).

### Windows (MinGW-w64)
- **Compiler**: `g++` with C++17 or higher
- **Dependencies included**: Dear ImGui, LuaJIT, ZeroMQ, FlatBuffers, GLFW
- Use the provided VS Code build task (`Ctrl+Shift+B` or run the task in `.vscode/tasks.json`).

---

Thank you for helping build **xONxX**!
