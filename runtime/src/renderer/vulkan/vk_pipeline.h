#pragma once
#include "vk_types.h"

b8 vk_graphics_pipeline_create(VulkanDevice* device, VkFormat* format, VulkanPipeline* out_pipeline);
void vk_pipeline_destroy(VulkanDevice* device, VulkanPipeline* pipeline);
