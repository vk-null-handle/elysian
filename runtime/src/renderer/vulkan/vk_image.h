#pragma once
#include "vk_types.h"

void vk_image_create(
	VulkanDevice* device,
	u32 width,
	u32 height,
	VkFormat format,
	VkImageUsageFlags usages,
	VkMemoryPropertyFlags memory_flags,
	b32 create_view,
	VkImageAspectFlags view_aspect_flags,
	VulkanImage* out_image);

void vk_image_view_create(
	VulkanDevice* device,
	VkFormat format,
	VulkanImage* image,
	VkImageAspectFlags aspect_flags);

void vk_image_destroy(VulkanDevice* device, VulkanImage* image);
void transition_image(VkCommandBuffer cmd_buffer, VkImage image, VkImageLayout old_layout, VkImageLayout new_layout);
