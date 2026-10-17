#pragma once
#include "defines.h"
#include "vk_types.h"

b8 vk_swapchain_create(VulkanDevice* device, VulkanSwapchain* swapchain, VkSurfaceKHR surface, VkExtent2D* extent);
void vk_swapchain_destroy(VulkanDevice* device, VulkanSwapchain* swapchain);
