#pragma once
#include "core/window.h"
#include "defines.h"

typedef enum renderer_backend_api {
	BACKEND_API_VULKAN,
	BACKEND_API_D3D12,
} BackendAPI;

typedef struct renderer_backend {
	b8 (*init)(Window* window);
	void (*shutdown)(void);
	void (*render_frame)(void);
} RendererBackend;
