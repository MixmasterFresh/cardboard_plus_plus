#include "Device.hpp"
#include <vector>
#include "validations.hpp"

VulkanDeviceInfo device;
VulkanSwapchainInfo swapchain;
VulkanBufferInfo buffers;
VulkanGfxPipelineInfo gfxPipeline;
VulkanRenderInfo render;

void CreateVulkanDevice(ANativeWindow* platformWindow, VkApplicationInfo* appInfo) {
  std::vector<const char*> instance_extensions;
  std::vector<const char*> device_extensions;

  instance_extensions.push_back("VK_KHR_surface");
  instance_extensions.push_back("VK_KHR_android_surface");

  device_extensions.push_back("VK_KHR_swapchain");


  VkInstanceCreateInfo instanceCreateInfo{
  .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
  .pNext = nullptr,
  .pApplicationInfo = appInfo,
  .enabledExtensionCount =
  static_cast<uint32_t>(instance_extensions.size()),
  .ppEnabledExtensionNames = instance_extensions.data(),
  .enabledLayerCount = 0,
  .ppEnabledLayerNames = nullptr,
  };
  CALL_VK(vkCreateInstance(&instanceCreateInfo, nullptr, &device.instance_));


  VkAndroidSurfaceCreateInfoKHR createInfo{
  .sType = VK_STRUCTURE_TYPE_ANDROID_SURFACE_CREATE_INFO_KHR,
  .pNext = nullptr,
  .flags = 0,
  .window = platformWindow
  };
  CALL_VK(vkCreateAndroidSurfaceKHR(device.instance_, &createInfo, nullptr, &device.surface_));


  // Find one GPU to use:
  // On Android, every GPU device is equal -- supporting
  // graphics/compute/present
  // for this sample, we use the very first GPU device found on the system
  uint32_t gpuCount = 0;
  CALL_VK(vkEnumeratePhysicalDevices(device.instance_, &gpuCount, nullptr));
  VkPhysicalDevice tmpGpus[gpuCount];
  CALL_VK(vkEnumeratePhysicalDevices(device.instance_, &gpuCount, tmpGpus));
  device.gpuDevice_ = tmpGpus[0];  // Pick up the first GPU Device

  //TODO: Select best GPU

  vkGetPhysicalDeviceMemoryProperties(device.gpuDevice_, &device.gpuMemoryProperties_);

  // Create a logical device (vulkan device)
  float priorities[] = {
  1.0f,
  };
  VkDeviceQueueCreateInfo queueCreateInfo{
    .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
    .pNext = nullptr,
    .flags = 0,
    .queueCount = 1,
    .queueFamilyIndex = 0,
    .pQueuePriorities = priorities,
  };

  VkDeviceCreateInfo deviceCreateInfo{
    .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
    .pNext = nullptr,
    .queueCreateInfoCount = 1,
    .pQueueCreateInfos = &queueCreateInfo,
    .enabledLayerCount = 0,
    .ppEnabledLayerNames = nullptr,
    .enabledExtensionCount = static_cast<uint32_t>(device_extensions.size()),
    .ppEnabledExtensionNames = device_extensions.data(),
    .pEnabledFeatures = nullptr,
  };

  CALL_VK(vkCreateDevice(device.gpuDevice_, &deviceCreateInfo, nullptr, &device.device_));
  vkGetDeviceQueue(device.device_, 0, 0, &device.queue_);
}
