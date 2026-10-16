#pragma once
#include "defines.h"
#include "vk_utils.h"
#include <vulkan/vulkan_core.h>

typedef struct vulkan_image {
    VkImage handle;
    VkDeviceMemory memory;
    VkImageView view;
    u32 width, height;
} VulkanImage;

typedef struct vulkan_swapchain {
  b8 require_recreate;
  u32 width, height;

  VkSurfaceFormatKHR image_format;
  VkSwapchainKHR handle;

  u32 n_images;
  VkImage *images;
  VkImageView *image_views;

  VulkanImage depth_image;
} VulkanSwapchain;

typedef struct vulkan_swapchain_support_info {
  VkSurfaceCapabilitiesKHR capabilities;
  u32 n_formats;
  VkSurfaceFormatKHR* formats;
  u32 n_present_modes;
  VkPresentModeKHR* present_modes;
} VulkanSwapchainSupportInfo;

typedef struct vulkan_device {
  VkDevice logical_dev;
  VkPhysicalDevice physical_dev;

  VkQueue gfx_queue;
  u32 gfx_queue_fam_idx;
  
  VkPhysicalDeviceProperties properties;
  VulkanSwapchainSupportInfo swapchain_info;
  
  VkFormat depth_format;
} VulkanDevice;

typedef struct vulkan_instance {
  VkInstance handle;
  const char** layers;
  const char** exts;
  u32 n_layers, n_ext;
} VulkanInstance;

typedef struct vulkan_context {
  VkAllocationCallbacks* allocator;
  VkSurfaceKHR surface;

  // Shaders
  VkShaderModule vert_shader;
  VkShaderModule frag_shader;

  VkSemaphore *render_complete;

  VulkanSwapchain swapchain;
  VulkanDevice device;
  VulkanInstance instance;
} VulkanContext;
