#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "defines.h"

typedef struct window {
  GLFWwindow *handle;
} Window;

b8 win_init(u32 width, u32 height, char *title, Window *out_window);
void win_shutdown(Window *window);

b8 win_should_close(const Window *window);
void win_get_framebuffer_size(Window* window, u32* out_width, u32* out_height);
void win_poll_events(void);

b8 win_create_vk_surface(Window* window, VkInstance instance, VkSurfaceKHR* out_surface);
const char **win_get_vk_instance_ext(u32 *count);

