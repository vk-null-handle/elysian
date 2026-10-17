#pragma once
#include <vulkan/vulkan_core.h>
#include "defines.h"
#include "vk_types.h"
#include <shaderc/shaderc.h>

VkShaderModule vk_shader_load(VulkanContext* vkcontext, const char* file_name, shaderc_shader_kind kind);

i32 find_memory_index(VulkanContext* vkcontext, u32 type_filter, u32 property_flags);

void _report_vk_assertion_failure(VkResult result, uint32_t line_num, const char* func_name, const char* file_name);
#ifdef DEBUG
#define VK_CHECK(result) _report_vk_assertion_failure(result, __LINE__, __func__, __FILE__)
#else
#define VK_CHECK(result) ((void)0);
#endif
