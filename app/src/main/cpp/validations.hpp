#ifndef CBPP_VALIDATIONS_H
#define CBPP_VALIDATIONS_H

#include <android/log.h>

static const char* kTAG = "Cardboard++";
#define LOGI(...) \
  ((void)__android_log_print(ANDROID_LOG_INFO, kTAG, __VA_ARGS__))
#define LOGW(...) \
  ((void)__android_log_print(ANDROID_LOG_WARN, kTAG, __VA_ARGS__))
#define LOGE(...) \
  ((void)__android_log_print(ANDROID_LOG_ERROR, kTAG, __VA_ARGS__))

// Vulkan call wrapper
#define CALL_VK(func)                                                 \
  if (VK_SUCCESS != (func)) {                                         \
    __android_log_print(ANDROID_LOG_ERROR, "CBPP ",               \
                        "Vulkan error. File[%s], line[%d]", __FILE__, \
                        __LINE__);                                    \
    assert(false);                                                    \
  }

// A macro to check value is VK_SUCCESS
// Used also for non-vulkan functions but return VK_SUCCESS
#define VK_CHECK(x) CALL_VK(x)

#endif
