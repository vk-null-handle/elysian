#include "vk_image.h"
#include "core/logger.h"
#include "renderer/vulkan/vk_types.h"
#include "vk_utils.h"
#include <vulkan/vulkan_core.h>

void vk_image_create(
	VulkanDevice* device,
	u32 width,
	u32 height,
	VkFormat format,
	VkImageUsageFlags usages,
	VkMemoryPropertyFlags memory_flags,
	b32 create_view,
	VkImageAspectFlags view_aspect_flags,
	VulkanImage* out_image) {

	out_image->width = width;
	out_image->height = height;

	// https://docs.vulkan.org/refpages/latest/refpages/source/VkImageCreateInfo.html
	VkImageCreateInfo image_create_info = {
		.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
		.imageType = VK_IMAGE_TYPE_2D,
		.extent = {
			.width = width,
			.height = height,
			.depth = 1,
		},
		.mipLevels = 1,
		.arrayLayers = 1,
		.format = format,
		.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
		.usage = usages,
		.samples = VK_SAMPLE_COUNT_1_BIT,
	};

	VK_CHECK(vkCreateImage(device->logical_dev, &image_create_info, NULL, &out_image->handle));

	// Query memory requirements for the image
	VkMemoryRequirements memory_requirements;
	vkGetImageMemoryRequirements(device->logical_dev, out_image->handle, &memory_requirements);

	// Find the memory type needed
	i32 memory_type = find_memory_index(device, memory_requirements.memoryTypeBits, memory_flags);
	if (memory_type == -1) {
		LOG_ERROR("Required memory type not found, Image not valid");
	}

	// https://docs.vulkan.org/refpages/latest/refpages/source/VkMemoryAllocateInfo.html
	VkMemoryAllocateInfo memory_allocate_info = {
		.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
		.allocationSize = memory_requirements.size,
		.memoryTypeIndex = memory_type,
	};
	// Allocate memory for image
	VK_CHECK(vkAllocateMemory(device->logical_dev, &memory_allocate_info, NULL, &out_image->memory));

	// Bind the memory
	VK_CHECK(vkBindImageMemory(device->logical_dev, out_image->handle, out_image->memory, 0));

	// Create view
	if (create_view) {
		out_image->view = 0;
		vk_image_view_create(device, format, out_image, view_aspect_flags);
	}
}

void vk_image_view_create(
	VulkanDevice* device,
	VkFormat format,
	VulkanImage* image,
	VkImageAspectFlags aspect_flags) {

	// https://docs.vulkan.org/refpages/latest/refpages/source/VkImageViewCreateInfo.html
	VkImageViewCreateInfo view_create_info = {
		.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
		.image = image->handle,
		.viewType = VK_IMAGE_VIEW_TYPE_2D,
		.format = format,
		.subresourceRange = {
			.aspectMask = aspect_flags,
			.baseMipLevel = 0,
			.baseArrayLayer = 0,
			.levelCount = 1,
			.layerCount = 1,
		},
	};

	VK_CHECK(vkCreateImageView(device->logical_dev, &view_create_info, NULL, &image->view));
}

void vk_image_destroy(VulkanDevice* device, VulkanImage* image) {
	if (image->view) {
		vkDestroyImageView(device->logical_dev, image->view, NULL);
		image->view = 0;
	}
	if (image->memory) {
		vkFreeMemory(device->logical_dev, image->memory, NULL);
		image->memory = 0;
	}
	if (image->handle) {
		vkDestroyImage(device->logical_dev, image->handle, NULL);
		image->handle = 0;
	}
}
