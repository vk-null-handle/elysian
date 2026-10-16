#include "vk_pipeline.h"
#include "core/logger.h"
#include "renderer/vulkan/vk_utils.h"
#include <vulkan/vulkan_core.h>

VkPipeline vk_graphics_pipeline_create(VulkanContext* vkcontext) {
	// Define pipeline layout
	VkPipelineLayoutCreateInfo pipeline_layout_info = {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
		// Has no descriptor sets or push constants
		.setLayoutCount = 0,
		.pushConstantRangeCount = 0,
	};
	VK_CHECK(vkCreatePipelineLayout(vkcontext->device.logical_dev, &pipeline_layout_info, vkcontext->allocator, &vkcontext->pipeline.layout));

	// Define main shader function
	const char* entry_point = "main";
	// Configure shader stages
	VkPipelineShaderStageCreateInfo shader_stages[] = {
		{
			.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
			.stage = VK_SHADER_STAGE_VERTEX_BIT,
			.module = vkcontext->vert_shader,
			.pName = entry_point,
		},
		{
			.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
			.stage = VK_SHADER_STAGE_FRAGMENT_BIT,
			.module = vkcontext->frag_shader,
			.pName = entry_point,
		},
	};
	u32 n_shader_stages = sizeof(shader_stages) / sizeof(shader_stages[0]);

	// Vertex pulling, don't define vertex input details
	VkPipelineVertexInputStateCreateInfo vert_input_info = {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
	};

	// Input assembly, tell Vulkan to draw triangle lists
	VkPipelineInputAssemblyStateCreateInfo input_assembly_info = {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
		.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
	};

	// Depth/Stencil configuration
	VkPipelineDepthStencilStateCreateInfo depth_stencil_info = {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
		.depthTestEnable = VK_TRUE,
		.depthWriteEnable = VK_TRUE,
		.depthCompareOp = VK_COMPARE_OP_LESS,
		.stencilTestEnable = VK_FALSE,
	};

	VkPipelineViewportStateCreateInfo viewport_info = {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
		.viewportCount = 1,
		.pViewports = NULL,
		.scissorCount = 1,
		.pScissors = NULL,
	};

	// Rasterizer settings
	VkPipelineRasterizationStateCreateInfo raster_info = {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
		// Full triangles
		.polygonMode = VK_POLYGON_MODE_FILL,
		.cullMode = VK_CULL_MODE_BACK_BIT,
		// Triangles vertices are clockwise
		.frontFace = VK_FRONT_FACE_CLOCKWISE,
		.lineWidth = 1.0f,
	};

	// No multisampling
	VkPipelineMultisampleStateCreateInfo multisample_info = {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
		.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
	};

	// Alpha blending (disabled)
	VkPipelineColorBlendAttachmentState attach_state = {
		.blendEnable = VK_FALSE,
		// Which colors to draw on swapchain
		.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
						  VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT,
	};
	VkPipelineColorBlendStateCreateInfo blend_info = {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
		.attachmentCount = 1,
		.pAttachments = &attach_state,
	};

	// We want to be able to change scissor and viewport at runtime
	VkDynamicState dynamic_state[] = {
		VK_DYNAMIC_STATE_VIEWPORT,
		VK_DYNAMIC_STATE_SCISSOR,
	};
	uint32_t n_dynamic_states = sizeof(dynamic_state) / sizeof(dynamic_state[0]);

	VkPipelineDynamicStateCreateInfo dynamic_state_info = {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
		.dynamicStateCount = n_dynamic_states,
		.pDynamicStates = dynamic_state,
	};

	// Structure required for dynamic rendering
	VkPipelineRenderingCreateInfo render_info = {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO,
		// Single color attachment (Swapchain)
		.colorAttachmentCount = 1,
		// Set swapchains color and devices depth formats
		.pColorAttachmentFormats = &vkcontext->swapchain.image_format.format,
		.depthAttachmentFormat = vkcontext->device.depth_format,
	};

	// Create the graphics pipeline
	VkGraphicsPipelineCreateInfo pipeline_info = {
		.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
		.pNext = &render_info,
		// Setup shader stages and associated them with vkshader modules
		.stageCount = n_shader_stages,
		.pStages = shader_stages,
		// Blank vertex input since vertexs in shader
		.pVertexInputState = &vert_input_info,
		// Input assembly state to tell pipeline we want triangle list topology
		.pInputAssemblyState = &input_assembly_info,
		// Tell Vulkan we want single viewport and scissor test, but didnt create them statically
		.pViewportState = &viewport_info,
		// Told rasterizer we want back faces culled and front faces filled
		.pRasterizationState = &raster_info,
		// Not using multisampling
		.pMultisampleState = &multisample_info,
		// Enabled depth testing and writing but not stenciling
		.pDepthStencilState = &depth_stencil_info,
		// Configured color blending (Disabled)
		.pColorBlendState = &blend_info,
		// Set viewport and scissor states to be dynamic
		.pDynamicState = &dynamic_state_info,
		// Created pipeline layout not expecting and descriptor sets or push constants
		.layout = vkcontext->pipeline.layout,
		// Disabled render passes
		.renderPass = VK_NULL_HANDLE,
	};

	VkPipeline pipeline;
	VK_CHECK(vkCreateGraphicsPipelines(vkcontext->device.logical_dev, NULL, 1, &pipeline_info, vkcontext->allocator, &pipeline));

	return pipeline;
}
