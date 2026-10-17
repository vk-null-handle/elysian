#pragma once
#include "defines.h"
#include "renderer/renderer_types.h"

b8 dx12_backend_init(Window* window);
void dx12_backend_shutdown(void);
void dx12_backend_render_frame(Window* window);
