# Engine
A simple, 3D only game engine written from the ground up using Vulkan 1.4 and C99.

## Start
To get started, clone the repository: `https://github.com/vk-null-handle/engine`.

You will need to have Clang or GCC, GLFW and the Vulkan SDK installed:
 - Clang: https://releases.llvm.org/download.html
 - GCC: https://gcc.gnu.org/releases.html
 - GLFW: https://www.glfw.org/download
 - Vulkan SDK: https://vulkan.lunarg.com/

Build project and generate compile commands with:
```
bear -- make
```

## Roadmap
- [ ] 3D renderer with PBR, reflections, shadows etc, made using modern Vulkan techniques
- [ ] Cross-platform support (Windows, Linux initially, Mac and Xbox later)
- [ ] Keyboard and mouse input, eventually gamepad
- [ ] Suite of game editor tools
- [ ] Lua game scripting and engine configuration
