#pragma once
#include "defines.h"
#include "vk_types.h"

void vk_device_query_swapchain_support_info(VkSurfaceKHR surface, VulkanDevice* device, VulkanSwapchainSupportInfo* out_info);
b8 vk_logical_dev_create(VulkanDevice* device, VkSurfaceKHR surface, VulkanInstance* instance);

static b8 find_phys_dev(VulkanInstance* instance, VulkanDevice* out_device);
static b8 find_graphics_queue(VulkanDevice* device, VkSurfaceKHR surface);
