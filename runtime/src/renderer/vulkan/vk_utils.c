#include "vk_utils.h"
#include <stdio.h>
#include <vulkan/vulkan_core.h>

#include "core/logger.h"
#include "core/memory.h"

i32 find_memory_index(VulkanDevice* device, u32 type_filter, u32 property_flags) {
	VkPhysicalDeviceMemoryProperties memory_properties;
	vkGetPhysicalDeviceMemoryProperties(device->physical_dev, &memory_properties);

	for (u32 i = 0; i < memory_properties.memoryTypeCount; ++i) {
		// Check each memory type to see if permitted and has the required properties
		if (type_filter & (1 << i) && (memory_properties.memoryTypes[i].propertyFlags & property_flags) == property_flags) {
			return i;
		}
	}

	LOG_WARN("Unable to find suitable memory type");
	return -1;
}

void transition_image(VkCommandBuffer cmd_buffer, VkImage image, VkImageLayout old_layout, VkImageLayout new_layout) {
	VkImageAspectFlags aspect_mask = new_layout == VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL ? VK_IMAGE_ASPECT_DEPTH_BIT : VK_IMAGE_ASPECT_COLOR_BIT;

	VkImageMemoryBarrier2 image_barrier = {
		.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2,
		.srcAccessMask = VK_ACCESS_2_COLOR_ATTACHMENT_WRITE_BIT,
		.dstAccessMask = 0,
		.srcStageMask = VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT,
		.dstStageMask = VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT,
		.subresourceRange = {
			.aspectMask = aspect_mask,
			.baseMipLevel = 0,
			.levelCount = 1,
			.baseArrayLayer = 0,
			.layerCount = 1,
		},
		.oldLayout = old_layout,
		.newLayout = new_layout,
		.image = image,
	};

	VkDependencyInfo dep_info = {
		.sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO,
		.pImageMemoryBarriers = &image_barrier,
		.imageMemoryBarrierCount = 1,
	};

	vkCmdPipelineBarrier2(cmd_buffer, &dep_info);
}

VkShaderModule vk_shader_load(VulkanDevice* device, const char* file_name, shaderc_shader_kind kind) {
	// read shader file from disk
	char shader_path[512];
	snprintf(shader_path, sizeof(shader_path), "../../runtime/assets/shaders/%s", file_name);

	FILE* file = fopen(shader_path, "rb");
	if (!file) {
		LOG_ERROR("Specified shader file doesn't exist (path: %s)", shader_path);
		return VK_NULL_HANDLE;
	}

	fseek(file, 0, SEEK_END);
	u32 file_size = ftell(file);
	rewind(file);

	char* src = mem_alloc(file_size + 1);
	if (!src) {
		fclose(file);
		return VK_NULL_HANDLE;
	}

	fread(src, 1, file_size, file);
	fclose(file);
	src[file_size] = '\0';

	// Compile the shader to SPIR-V
	LOG_DEBUG("Compiling shader: %s", shader_path);

	shaderc_compiler_t compiler = shaderc_compiler_initialize();

	shaderc_compile_options_t opts = shaderc_compile_options_initialize();

	shaderc_compile_options_set_target_env(opts, shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_4);
	shaderc_compile_options_set_target_spirv(opts, shaderc_spirv_version_1_6);
	shaderc_compile_options_set_optimization_level(opts, shaderc_optimization_level_performance);

	shaderc_compilation_result_t result = shaderc_compile_into_spv(compiler, src, file_size, kind, file_name, "main", opts);

	mem_free(src, file_size + 1);

	if (shaderc_result_get_compilation_status(result) != shaderc_compilation_status_success) {
		LOG_ERROR("Vulkan shader compilation error: %s", shaderc_result_get_error_message(result));

		shaderc_result_release(result);
		shaderc_compile_options_release(opts);
		shaderc_compiler_release(compiler);

		return VK_NULL_HANDLE;
	}

	// Pass SPIR-V to Vulkan and create shader-module
	VkShaderModuleCreateInfo module_create_info = {
		.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
		.codeSize = shaderc_result_get_length(result),
		.pCode = (const u32*)shaderc_result_get_bytes(result),
	};

	VkShaderModule shader_module = VK_NULL_HANDLE;

	if (vkCreateShaderModule(device->logical_dev, &module_create_info, NULL, &shader_module) != VK_SUCCESS) {
		LOG_ERROR("Failed to create Vulkan shader module");

		shaderc_result_release(result);
		shaderc_compile_options_release(opts);
		shaderc_compiler_release(compiler);

		return VK_NULL_HANDLE;
	}

	shaderc_result_release(result);
	shaderc_compile_options_release(opts);
	shaderc_compiler_release(compiler);

	return shader_module;
}
