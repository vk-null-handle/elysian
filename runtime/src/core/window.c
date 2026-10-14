#include "window.h"
#include "core/logger.h"
#include "defines.h"

b8 win_init(u32 width, u32 height, char* title, Window* out_window) {
	if (!out_window || !glfwInit()) {
		return FALSE;
	}
	if (!glfwVulkanSupported()) {
		LOG_ERROR("Vulkan is not supported on this system");
		glfwTerminate();
		return FALSE;
	}

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	out_window->handle = glfwCreateWindow(width, height, title, NULL, NULL);

	if (out_window->handle == NULL) {
		glfwTerminate();
		return FALSE;
	}
	glfwSetWindowUserPointer(out_window->handle, out_window);
	return TRUE;
}

void win_shutdown(Window* window) {
	if (!window) {
		return;
	}

	glfwDestroyWindow(window->handle);
	glfwTerminate();
}

b8 win_should_close(const Window* window) {
	return glfwWindowShouldClose(window->handle);
}

void win_get_framebuffer_size(Window* window, i32* out_width, i32* out_height) {
	glfwGetFramebufferSize(window->handle, out_width, out_height);
}

void win_poll_events(void) {
	glfwPollEvents();
}

b8 win_create_vk_surface(Window* window, VkInstance instance, VkSurfaceKHR* out_surface) {
	return glfwCreateWindowSurface(instance, window->handle, NULL, out_surface);
}

const char** win_get_instance_ext(u32* count) {
	return glfwGetRequiredInstanceExtensions(count);
}
