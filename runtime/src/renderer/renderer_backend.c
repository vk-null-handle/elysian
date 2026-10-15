#include "renderer_backend.h"
#include "core/logger.h"
#include "directx12/dx12_backend.h"
#include "vulkan/vk_backend.h"

b8 renderer_backend_create(BackendAPI api, RendererBackend* out_backend) {

	if (api == BACKEND_API_VULKAN) {
		out_backend->init = vulkan_backend_init;
		out_backend->shutdown = vulkan_backend_shutdown;
		out_backend->render_frame = vulkan_backend_render_frame;

		return TRUE;
	} else if (api == BACKEND_API_D3D12) {
		out_backend->init = dx12_backend_init;
		out_backend->shutdown = dx12_backend_shutdown;
		out_backend->render_frame = dx12_backend_render_frame;

		LOG_ERROR("DirectX12 not implemented");
		return FALSE;
	}

	return FALSE;
}

void renderer_backend_destroy(RendererBackend* renderer_backend) {
	renderer_backend->init = 0;
	renderer_backend->shutdown = 0;
	renderer_backend->render_frame = 0;
}
