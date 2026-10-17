#pragma once
#include <vulkan/vulkan_core.h>
#include "defines.h"
#include "vk_types.h"
#include "core/assert.h"
#include <shaderc/shaderc.h>
#include <vulkan/vk_enum_string_helper.h>

VkShaderModule vk_shader_load(VulkanDevice* device, const char* file_name, shaderc_shader_kind kind);

i32 find_memory_index(VulkanDevice* device, u32 type_filter, u32 property_flags);

#define VK_CHECK(expr) { \
  if (expr == VK_SUCCESS) { \
  } else { \
    _report_assertion_failure(#expr, string_VkResult(expr), __FILE__, __LINE__); \
    debug_break() \
  } \
} \
