#include "vk_swapchain.h"
#include "core/logger.h"
#include "vk_utils.h"
#include "vk_device.h"
#include "vk_image.h"

#include "core/memory.h"
#include <vulkan/vulkan_core.h>

void vk_swapchain_destroy(VulkanContext* vkcontext, VulkanSwapchain* swapchain) {
	// Destory each swapchain images semaphore and image view
	for (u32 i = 0; i < swapchain->n_images; i++) {
		vkDestroyImageView(vkcontext->device.logical_dev, swapchain->image_views[i], NULL);
	}
	// Free semaphores, images, and image views
	mem_free(swapchain->images, swapchain->n_images * sizeof(VkImage));
	mem_free(swapchain->image_views, swapchain->n_images * sizeof(VkImageView));
	// Destroy depth image
	vk_image_destroy(&vkcontext->device, &swapchain->depth_image);

	vkDestroySwapchainKHR(vkcontext->device.logical_dev, swapchain->handle, NULL);
}

void vk_swapchain_create(VulkanContext* vkcontext, VulkanSwapchain* swapchain, u32 width, u32 height) {
	LOG_DEBUG("HERE");
	swapchain->width = width;
	swapchain->height = height;
	// Get device support info
	vk_device_query_swapchain_support_info(vkcontext->surface, &vkcontext->device, &vkcontext->device.swapchain_info);

	// Choose a swap surface format.
	b8 supported = FALSE;
	for (u32 i = 0; i < vkcontext->device.swapchain_info.n_formats; ++i) {
		VkSurfaceFormatKHR format = vkcontext->device.swapchain_info.formats[i];
		// Preferred formats
		if (format.format == VK_FORMAT_B8G8R8A8_UNORM &&
			format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
			swapchain->image_format = format;
			supported = TRUE;
			break;
		}
	}
	// Default to whatever the first format is
	if (!supported) {
		swapchain->image_format = vkcontext->device.swapchain_info.formats[0];
	}

	// Set fifo_khr as default (its always supported)
	VkPresentModeKHR present_mode = VK_PRESENT_MODE_FIFO_KHR;
	for (u32 i = 0; i < vkcontext->device.swapchain_info.n_present_modes; ++i) {
		VkPresentModeKHR mode = vkcontext->device.swapchain_info.present_modes[i];
		// If mailbox_khr supported then use it instead
		if (mode == VK_PRESENT_MODE_MAILBOX_KHR) {
			present_mode = mode;
			break;
		}
	}

	// Fill surface capabilities struct so we can query min number of images for swapchain
	VkSurfaceCapabilitiesKHR surface_caps = {};
	VK_CHECK(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(vkcontext->device.physical_dev, vkcontext->surface, &surface_caps));

	// Clamp extent
	if (surface_caps.currentExtent.width != UINT32_MAX) {
		swapchain->width = surface_caps.currentExtent.width;
		swapchain->height = surface_caps.currentExtent.height;
	} else {
		swapchain->width = CLAMP(
			width,
			surface_caps.minImageExtent.width,
			surface_caps.maxImageExtent.width);

		swapchain->height = CLAMP(
			height,
			surface_caps.minImageExtent.height,
			surface_caps.maxImageExtent.height);
	}

	// https://docs.vulkan.org/refpages/latest/refpages/source/VkSwapchainCreateInfoKHR.html
	VkSwapchainCreateInfoKHR swapchain_create_info = {
		.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
		.surface = vkcontext->surface,
		.minImageCount = surface_caps.minImageCount,
		.imageFormat = swapchain->image_format.format,
		.imageColorSpace = swapchain->image_format.colorSpace,
		.imageExtent = {
			.width = swapchain->width,
			.height = swapchain->height,
		},
		.imageArrayLayers = 1,
		.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
		.preTransform = surface_caps.currentTransform,
		.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
		.presentMode = present_mode,
		// Using same queues for simplicity
		.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
	};

	VK_CHECK(vkCreateSwapchainKHR(vkcontext->device.logical_dev, &swapchain_create_info, NULL, &swapchain->handle));

	// Get images
	vkGetSwapchainImagesKHR(vkcontext->device.logical_dev, swapchain->handle, &swapchain->n_images, NULL);
	swapchain->images = mem_calloc(swapchain->n_images, sizeof(VkImage));
	vkGetSwapchainImagesKHR(vkcontext->device.logical_dev, swapchain->handle, &swapchain->n_images, swapchain->images);

	// Create swapchain image view for each swapchain image
	swapchain->image_views = mem_calloc(swapchain->n_images, sizeof(VkImageView));
	for (u32 i = 0; i < swapchain->n_images; i++) {
		// https://docs.vulkan.org/refpages/latest/refpages/source/VkImageViewCreateInfo.html
		VkImageViewCreateInfo image_view_info = {
			.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
			.image = swapchain->images[i],
			.viewType = VK_IMAGE_VIEW_TYPE_2D,
			.format = swapchain->image_format.format,
			.subresourceRange = {
				.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
				.baseMipLevel = 0,
				.levelCount = 1,
				.baseArrayLayer = 0,
				.layerCount = 1,
			},
		};

		VK_CHECK(vkCreateImageView(vkcontext->device.logical_dev, &image_view_info, NULL, &swapchain->image_views[i]));
	}

	// Create semaphore to signal render completion for each swapchain image
	vkcontext->render_complete = mem_calloc(swapchain->n_images, sizeof(*vkcontext->render_complete));
	for (u32 i = 0; i < swapchain->n_images; i++) {
		// https://docs.vulkan.org/refpages/latest/refpages/source/VkSemaphoreCreateInfo.html
		const VkSemaphoreCreateInfo sem_info = {
			.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
		};
		VK_CHECK(vkCreateSemaphore(vkcontext->device.logical_dev, &sem_info, NULL, &vkcontext->render_complete[i]));
	}

	// Create depth image
	vk_image_create(
		&vkcontext->device,
		swapchain->width,
		swapchain->height,
		vkcontext->device.depth_format,
		VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		TRUE,
		VK_IMAGE_ASPECT_DEPTH_BIT,
		&swapchain->depth_image);
}
