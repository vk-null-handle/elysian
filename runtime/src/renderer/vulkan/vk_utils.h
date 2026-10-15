#pragma once
#include <vulkan/vulkan_core.h>

void _report_vk_assertion_failure(VkResult result, uint32_t line_num, const char* func_name, const char* file_name);

#ifdef DEBUG
#define VK_CHECK(result) _report_vk_assertion_failure(result, __LINE__, __func__, __FILE__)
#else
#define VK_CHECK(result) ((void)0);
#endif
