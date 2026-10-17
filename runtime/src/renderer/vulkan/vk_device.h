#pragma once
#include "defines.h"
#include "vk_types.h"

void vk_device_query_swapchain_support_info(VkSurfaceKHR surface, VulkanDevice* device, VulkanSwapchainSupportInfo* out_info);
b8 vk_logical_dev_create(VulkanContext* vkcontext);

static b8 find_phys_dev(VulkanContext* vkcontext);
static b8 find_graphics_queue(VulkanContext* vkcontext);
