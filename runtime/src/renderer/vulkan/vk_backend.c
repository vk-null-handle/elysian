#include "vk_backend.h"
#include "vk_types.h"
#include <vulkan/vulkan_core.h>

// TEMP
static VulkanContext vkcontext;

b8 vulkan_backend_init(Window* window) {
	static const char* layers[] = {"VK_LAYER_KHRONOS_validation"};
	u32 n_instance_exts;
	const char** exts = win_get_vk_instance_ext(&n_instance_exts);

	// https://docs.vulkan.org/refpages/latest/refpages/source/VkApplicationInfo.html
	VkApplicationInfo app_info = {
		.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
		.pApplicationName = "Engine",
		.applicationVersion = VK_MAKE_VERSION(0, 1, 0),
		.apiVersion = VK_API_VERSION_1_4,
	};

	// https://docs.vulkan.org/refpages/latest/refpages/source/VkInstanceCreateInfo.html
	VkInstanceCreateInfo inst_info = {
		.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
		.pApplicationInfo = &app_info,
		.enabledLayerCount = 1,
		.ppEnabledLayerNames = layers,
		.enabledExtensionCount = n_instance_exts,
		.ppEnabledExtensionNames = exts,
	};

	VK_CHECK(vkCreateInstance(&inst_info, vkcontext.allocator, &vkcontext.instance.handle));
	LOG_DEBUG("Created instance");

	LOG_DEBUG("Vulkan Initialized");
	return TRUE;
}

void vulkan_backend_shutdown(void) {
}

void vulkan_backend_render_frame(void) {
}
