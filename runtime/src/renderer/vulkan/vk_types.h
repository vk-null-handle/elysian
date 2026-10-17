#pragma once
#include "defines.h"
#include "vk_utils.h"
#include <vulkan/vulkan_core.h>

#define MAX_FRAMES_IN_FLIGHT 2

typedef enum vulkan_cmd_buffer_state {
  COMMAND_BUFFER_STATE_READY,
  COMMAND_BUFFER_STATE_RECORDING,
  COMMAND_BUFFER_STATE_RECORDING_ENDED,
  COMMAND_BUFFER_STATE_IN_RENDERING,
  COMMAND_BUFFER_STATE_RENDERING_ENDED,
  COMMAND_BUFFER_STATE_SUBMITTED,
  COMMAND_BUFFER_STATE_NOT_ALLOCATED
} VulkanCmdBufferState;

typedef struct vulkan_command_buffer {
    VkCommandBuffer handle;
    VulkanCmdBufferState state;
} VulkanCmdBuffer;

// Per frame resources
typedef struct frame_resources {
	VkCommandPool command_pool;
	VulkanCmdBuffer command_buffer;
	VkSemaphore image_acquired;
} FrameResources;

typedef struct vulkan_pipeline {
  VkPipeline handle;
  VkPipelineLayout layout;
  // Shaders
  VkShaderModule vert_shader;
  VkShaderModule comp_shader;
  VkShaderModule frag_shader;
} VulkanPipeline;

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

  VkSemaphore timeline;
  u64 next_signal_value;
  u64 frame_index;
  VkSemaphore *render_complete;
  FrameResources frame_resources[MAX_FRAMES_IN_FLIGHT];

  VulkanSwapchain swapchain;
  VulkanDevice device;
  VulkanInstance instance;
  VulkanPipeline pipeline;
} VulkanContext;
