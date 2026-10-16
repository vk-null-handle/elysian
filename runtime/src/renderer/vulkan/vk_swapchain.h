#pragma once
#include "defines.h"
#include "vk_types.h"

void vk_swapchain_create(VulkanContext* vkcontext, VulkanSwapchain* swapchain, u32 width, u32 height);
void vk_swapchain_destroy(VulkanContext* vkcontext, VulkanSwapchain* swapchain);
