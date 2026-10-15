#pragma once
#include "defines.h"
#include "renderer/renderer_types.h"

b8 vulkan_backend_init(Window *window);
void vulkan_backend_shutdown(void);

void vulkan_backend_render_frame(void);
