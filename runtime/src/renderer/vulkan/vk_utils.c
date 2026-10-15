#include "vk_utils.h"
#include "core/logger.h"
#include <vulkan/vk_enum_string_helper.h>
#include "core/assert.h"

void _report_vk_assertion_failure(VkResult result, uint32_t line_num, const char* func_name, const char* file_name) {
	if (result != VK_SUCCESS) {
		char* msg = "Vulkan Assertion Failure: %s, message: %s, file: %s, line: %d";
		LOG_ERROR(msg, func_name, string_VkResult(result), file_name, line_num);
		debug_break();
	} else {
		LOG_TRACE("%s, %s", func_name, string_VkResult(result));
	}
}
