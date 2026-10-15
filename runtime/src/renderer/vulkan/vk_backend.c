#include "vk_backend.h"
#include "core/logger.h"
#include <vulkan/vulkan_core.h>

b8 vulkan_backend_init(Window* window) {
	LOG_DEBUG("Vulkan Initialized");
	return TRUE;
}

void vulkan_backend_shutdown(void) {
}

void vulkan_backend_render_frame(void) {
}
