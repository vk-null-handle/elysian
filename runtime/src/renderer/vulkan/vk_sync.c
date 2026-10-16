#include "vk_sync.h"
#include "vk_utils.h"
#include <vulkan/vulkan_core.h>

void vk_sync_resources_create(VulkanContext* vkcontext) {
	// https://docs.vulkan.org/refpages/latest/refpages/source/VkSemaphoreTypeCreateInfo.html
	VkSemaphoreTypeCreateInfo semaphore_type_info = {
		.sType = VK_STRUCTURE_TYPE_SEMAPHORE_TYPE_CREATE_INFO,
		// Set to timeline semaphore
		.semaphoreType = VK_SEMAPHORE_TYPE_TIMELINE,
		.initialValue = MAX_FRAMES_IN_FLIGHT,
	};

	// https://docs.vulkan.org/refpages/latest/refpages/source/VkSemaphoreCreateInfo.html
	VkSemaphoreCreateInfo semaphore_info = {
		.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
		// Pass in type info
		.pNext = &semaphore_type_info,
	};

	VK_CHECK(vkCreateSemaphore(vkcontext->device.logical_dev, &semaphore_info, vkcontext->allocator, &vkcontext->timeline));

	// Per-frame image-acquire semaphores
	for (u8 i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i) {
		// Create binary sempaphores
		struct frame_resources* resources = &vkcontext->frame_resources[i];
		VkSemaphoreCreateInfo semaphore_info = {
			.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
		};
		VK_CHECK(vkCreateSemaphore(vkcontext->device.logical_dev, &semaphore_info, vkcontext->allocator, &resources->image_acquired));
	}
}
