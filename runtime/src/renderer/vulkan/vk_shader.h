#pragma once
#include "vk_types.h"
#include <shaderc/shaderc.h>

VkShaderModule vk_shader_load(VulkanContext* vkcontext, const char* file_name, shaderc_shader_kind kind);
