#pragma once
#include "renderer_types.h"

b8 renderer_backend_create(BackendAPI api, RendererBackend* out_backend);
void renderer_backend_destroy(RendererBackend* renderer_backend);
