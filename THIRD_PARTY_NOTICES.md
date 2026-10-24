# Third-Party Notices & Licenses

The **xONxX Engine** (*eXtensive Open Noumenal eXploration & eXperimentation*) bundles or incorporates components from the following open-source projects. We gratefully acknowledge the creators and contributors of these libraries.

---

## Table of Contents
1. [Dear ImGui](#1-dear-imgui) (MIT)
2. [ImGuizmo](#2-imguizmo) (MIT)
3. [GLM (OpenGL Mathematics)](#3-glm-opengl-mathematics) (MIT / Happy Bunny)
4. [LuaJIT & Lua](#4-luajit--lua) (MIT)
5. [ZeroMQ (libzmq & cppzmq)](#5-zeromq-libzmq--cppzmq) (MPL 2.0)
6. [FlatBuffers](#6-flatbuffers) (Apache 2.0)
7. [GLFW](#7-glfw) (zlib/libpng)
8. [stb_image](#8-stb_image) (Public Domain / MIT)
9. [libsodium](#9-libsodium) (ISC)
10. [GLAD](#10-glad) (Public Domain / MIT / Apache 2.0)

---

## 1. Dear ImGui
- **Source**: `cpp_engine/imgui/`
- **Author**: Omar Cornut and Dear ImGui contributors
- **URL**: https://github.com/ocornut/imgui
- **License**: MIT License

```
The MIT License (MIT)

Copyright (c) 2014-2024 Omar Cornut

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```

---

## 2. ImGuizmo
- **Source**: `cpp_engine/imgui/ImGuizmo.h`, `ImGuizmo.cpp`
- **Author**: Cedric Guillemet
- **URL**: https://github.com/CedricGuillemet/ImGuizmo
- **License**: MIT License

```
The MIT License (MIT)

Copyright (c) 2016 Cedric Guillemet

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```

---

## 3. GLM (OpenGL Mathematics)
- **Source**: `cpp_engine/include/glm/`
- **Author**: G-Truc Creation
- **URL**: https://github.com/g-truc/glm
- **License**: The Happy Bunny License / MIT License

```
The MIT License (MIT)

Copyright (c) 2005 - present G-Truc Creation

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```

---

## 4. LuaJIT & Lua
- **Source**: `cpp_engine/include/lua/`, `cpp_engine/lib/libluajit.a`
- **Author**: Mike Pall (LuaJIT) / PUC-Rio (Lua)
- **URL**: https://luajit.org / https://www.lua.org
- **License**: MIT License

```
LuaJIT -- a Just-In-Time Compiler for Lua. https://luajit.org/
Copyright (C) 2005-2023 Mike Pall. All rights reserved.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
```

---

## 5. ZeroMQ (libzmq & cppzmq)
- **Source**: `cpp_engine/src/zmq.h`, `zmq.hpp`, `cpp_engine/libzmq.dll`
- **Author**: iMatix Corporation and ZeroMQ contributors
- **URL**: https://zeromq.org / https://github.com/zeromq/libzmq
- **License**: Mozilla Public License Version 2.0 (MPL 2.0)

```
Mozilla Public License Version 2.0
==================================

1. Definitions
--------------
...
Full license text available at: https://www.mozilla.org/en-US/MPL/2.0/

libzmq is licensed under the Mozilla Public License 2.0 (MPL-2.0).
ZeroMQ is dynamically linked via `libzmq.dll`. Your use of xONxX does
not modify the source of ZeroMQ itself. If you modify libzmq source code,
the MPL-2.0 requires you to make those modifications available.
```

---

## 6. FlatBuffers
- **Source**: `cpp_engine/src/flatbuffers/`, `schemas/flatc.exe`
- **Author**: Google LLC
- **URL**: https://github.com/google/flatbuffers
- **License**: Apache License, Version 2.0

```
Copyright 2014 Google Inc.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
```

---

## 7. GLFW
- **Source**: `cpp_engine/lib/libglfw3dll.a`
- **Author**: Marcus Geelnard, Camilla Löwy, and GLFW contributors
- **URL**: https://www.glfw.org
- **License**: zlib/libpng License

```
Copyright (c) 2002-2006 Marcus Geelnard
Copyright (c) 2006-2019 Camilla Löwy <elmindreda@glfw.org>

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
   claim that you wrote the original software. If you use this software
   in a product, an acknowledgment in the product documentation would be
   appreciated but is not required.

2. Altered source versions must be plainly marked as such, and must not be
   misrepresented as being the original software.

3. This notice may not be removed or altered from any source distribution.
```

---

## 8. stb_image
- **Source**: `cpp_engine/include/stb_image.h`
- **Author**: Sean Barrett
- **URL**: https://github.com/nothings/stb
- **License**: MIT License / Public Domain (Dual-Licensed)

```
This software is dual-licensed to the public domain and under the following
license: you are granted a perpetual, irrevocable license to copy, modify,
publish, and distribute this file as you see fit.
```

---

## 9. libsodium
- **Source**: `cpp_engine/libsodium.dll`
- **Author**: Frank Denis and contributors
- **URL**: https://github.com/jedisct1/libsodium
- **License**: ISC License

```
ISC License

Copyright (c) 2013-2024
Frank Denis <j at pureftpd dot org>

Permission to use, copy, modify, and/or distribute this software for any
purpose with or without fee is hereby granted, provided that the above
copyright notice and this permission notice appear in all copies.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
```

---

## 10. GLAD (OpenGL Loader)
- **Source**: `cpp_engine/src/glad.c`
- **Generator**: Dav1dde (https://glad.dav1d.de/)
- **Specification**: Khronos Group
- **License**: Public Domain / MIT
