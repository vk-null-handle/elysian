#include "vk_backend.h"
#include "core/logger.h"
#include "core/assert.h"
#include "core/memory.h"

#include "vk_utils.h"
#include "vk_swapchain.h"
#include "vk_types.h"
#include "vk_device.h"
#include "vk_pipeline.h"
#include "vk_sync.h"
#include "vk_cmd_buffer.h"
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

	VK_CHECK(vkCreateInstance(&inst_info, NULL, &vkcontext.instance.handle));
	LOG_DEBUG("Created Vulkan instance");

	VK_CHECK(win_create_vk_surface(window, vkcontext.instance.handle, &vkcontext.surface));
	LOG_DEBUG("Created Vulkan surface");

	ASSERT(vk_device_create(&vkcontext.device, vkcontext.surface, &vkcontext.instance));
	LOG_DEBUG("Created Vulkan logical device");

	VkExtent2D extent;
	win_get_framebuffer_size(window, &extent.width, &extent.height);
	if (extent.width == 0 || extent.height == 0) {
		extent.width = 1920;
		extent.height = 800;
	}

	ASSERT(vk_swapchain_create(&vkcontext.device, &vkcontext.swapchain, vkcontext.surface, &extent));
	LOG_DEBUG("Created Vulkan swapchain");

	vkcontext.pipeline.vert_shader = vk_shader_load(&vkcontext.device, "shader_vert.glsl", shaderc_vertex_shader);
	vkcontext.pipeline.frag_shader = vk_shader_load(&vkcontext.device, "shader_frag.glsl", shaderc_fragment_shader);
	ASSERT(vk_graphics_pipeline_create(&vkcontext.device, &vkcontext.pipeline));
	LOG_DEBUG("Created Vulkan graphics pipeline");

	vk_sync_resources_create(&vkcontext);
	LOG_DEBUG("Created Vulkan sync resources");

	vk_command_buffers_create(&vkcontext);
	LOG_DEBUG("Created Vulkan command buffers and pool");

	return TRUE;
}

void vulkan_backend_shutdown(void) {
	VK_CHECK(vkDeviceWaitIdle(vkcontext.device.logical_dev));

	if (vkcontext.timeline) {
		vkDestroySemaphore(vkcontext.device.logical_dev, vkcontext.timeline, NULL);
	}

	for (u8 i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i) {
		struct frame_resources* frame = &vkcontext.frame_resources[i];
		if (frame->image_acquired) {
			vkDestroySemaphore(vkcontext.device.logical_dev, frame->image_acquired, NULL);
		}
	}

	vk_pipeline_destroy(&vkcontext.device, &vkcontext.pipeline);

	if (vkcontext.render_complete) {
		for (u32 i = 0; i < vkcontext.swapchain.n_images; i++) {
			vkDestroySemaphore(vkcontext.device.logical_dev, vkcontext.render_complete[i], NULL);
		}
		mem_free(vkcontext.render_complete, vkcontext.swapchain.n_images * sizeof(*vkcontext.render_complete));
	}

	vk_swapchain_destroy(&vkcontext.device, &vkcontext.swapchain);

	if (vkcontext.surface) {
		vkDestroySurfaceKHR(vkcontext.instance.handle, vkcontext.surface, NULL);
	};

	vk_device_destroy(&vkcontext.device);

	if (vkcontext.instance.handle) {
		vkDestroyInstance(vkcontext.instance.handle, NULL);
	}
}

void vulkan_backend_render_frame(Window* window) {
}
