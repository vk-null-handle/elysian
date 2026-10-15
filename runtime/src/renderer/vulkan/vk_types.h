#pragma once
#include "defines.h"
#include "core/logger.h"
#include "core/assert.h"
#include "vk_utils.h"
#include <vulkan/vulkan_core.h>

typedef struct vulkan_instance {
  VkInstance handle;
  const char** layers;
  const char** exts;
  u32 n_layers, n_ext;
} VulkanInstance;

typedef struct vulkan_context {
  VkAllocationCallbacks* allocator;
  
  VulkanInstance instance;
} VulkanContext;
