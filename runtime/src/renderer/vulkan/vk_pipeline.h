#pragma once
#include "vk_types.h"

b8 vk_graphics_pipeline_create(VulkanDevice* device, VulkanPipeline* out_pipeline, VkAllocationCallbacks* allocator);
void vk_pipeline_destroy(VulkanDevice* device, VulkanPipeline* pipeline, VkAllocationCallbacks* allocator);
