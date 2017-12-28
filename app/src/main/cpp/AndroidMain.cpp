// Copyright 2016 Google Inc. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
#include <android/log.h>
#include <android_native_app_glue.h>
#include "CardboardRenderer.hpp"

static CardboardRenderer *renderer = nullptr;

// Process the next main command.
void handle_cmd(android_app* app, int32_t cmd) {
  __android_log_print(ANDROID_LOG_INFO, "CBPP",
  "event received: %d", cmd);
  switch (cmd) {
    case APP_CMD_INIT_WINDOW:
      // The window is being shown, get it ready.
      renderer->initVulkan();
      break;
    case APP_CMD_TERM_WINDOW:
      // The window is being hidden or closed, clean it up.
      renderer->cleanup();
      break;
    default:
      __android_log_print(ANDROID_LOG_INFO, "CBPP",
                          "event not handled: %d", cmd);
  }
}

void android_main(struct android_app* app) {
  renderer = new CardboardRenderer(app);
  // Set the callback to process system events
  app->onAppCmd = handle_cmd;
  renderer->mainLoop();
}
