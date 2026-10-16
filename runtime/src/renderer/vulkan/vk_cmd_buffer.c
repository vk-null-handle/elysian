#include "vk_cmd_buffer.h"
#include "vk_utils.h"
#include <vulkan/vulkan_core.h>

void vk_command_buffers_create(VulkanContext* vkcontext) {
	// Create command pool & buffer for each frame in flight
	for (u8 i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i) {
		struct frame_resources* frame = &vkcontext->frame_resources[i];

		// https://docs.vulkan.org/refpages/latest/refpages/source/VkCommandPoolCreateInfo.html
		VkCommandPoolCreateInfo pool_info = {
			.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
			.queueFamilyIndex = vkcontext->device.gfx_queue_fam_idx,
		};

		VK_CHECK(vkCreateCommandPool(vkcontext->device.logical_dev, &pool_info, vkcontext->allocator, &frame->command_pool));

		// https://docs.vulkan.org/refpages/latest/refpages/source/VkCommandBufferAllocateInfo.html
		VkCommandBufferAllocateInfo cmd_alloc_info = {
			.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
			.commandPool = frame->command_pool,
			.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
			// Only have one for now
			.commandBufferCount = 1,
		};

		// Set state
		frame->command_buffer.state = COMMAND_BUFFER_STATE_NOT_ALLOCATED;
		VK_CHECK(vkAllocateCommandBuffers(vkcontext->device.logical_dev, &cmd_alloc_info, &frame->command_buffer.handle));
		frame->command_buffer.state = COMMAND_BUFFER_STATE_READY;
	}
}
