#pragma once
#include "vk_types.h"

void vk_image_create(
	VulkanContext* vkcontext,
	u32 width,
	u32 height,
	VkFormat format,
	VkImageUsageFlags usages,
	VkMemoryPropertyFlags memory_flags,
	b32 create_view,
	VkImageAspectFlags view_aspect_flags,
	VulkanImage* out_image);

void vk_image_view_create(VulkanContext* context,
    VkFormat format,
    VulkanImage* image,
    VkImageAspectFlags aspect_flags);

void vk_image_destroy(VulkanContext* context, VulkanImage* image);
