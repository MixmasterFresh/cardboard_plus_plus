#ifndef CBPP_DEVICE_H
#define CBPP_DEVICE_H

#include "vulkan_wrapper.h"
#include <android_native_app_glue.h>

struct VulkanSwapchainInfo {
  VkSwapchainKHR swapchain_;
  uint32_t swapchainLength_;
  VkExtent2D displaySize_;
  VkFormat displayFormat_;
  VkFramebuffer* framebuffers_; // array of frame buffers and views
  VkImageView* displayViews_;
};


struct VulkanDeviceInfo {
  bool initialized_;
  VkInstance instance_;
  VkPhysicalDevice gpuDevice_;
  VkPhysicalDeviceMemoryProperties gpuMemoryProperties_;
  VkDevice device_;
  VkSurfaceKHR surface_;
  VkQueue queue_;
};


typedef struct texture_object {
  VkSampler sampler;
  VkImage image;
  VkImageLayout imageLayout;
  VkDeviceMemory mem;
  VkImageView view;
  int32_t tex_width;
  int32_t tex_height;
} texture_object;


struct VulkanBufferInfo {
  VkBuffer vertexBuf_;
};


struct VulkanGfxPipelineInfo {
  VkDescriptorSetLayout dscLayout_;
  VkDescriptorPool descPool_;
  VkDescriptorSet descSet_;
  VkPipelineLayout layout_;
  VkPipelineCache cache_;
  VkPipeline pipeline_;
};


struct VulkanRenderInfo {
  VkRenderPass renderPass_;
  VkCommandPool cmdPool_;
  VkCommandBuffer* cmdBuffer_;
  uint32_t cmdBufferLen_;
  VkSemaphore semaphore_;
  VkFence fence_;
};


extern VulkanDeviceInfo device;
extern VulkanSwapchainInfo swapchain;
extern VulkanBufferInfo buffers;
extern VulkanGfxPipelineInfo gfxPipeline;
extern VulkanRenderInfo render;


void CreateVulkanDevice(ANativeWindow* platformWindow, VkApplicationInfo* appInfo);

#endif
