#pragma once
/* ---------------------------------------------------------------------
 * Copyright (c) 2026 Ajeet Singh Yadav. All rights reserved.
 * Licensed under the Apache License, Version 2.0 (the "License")
 *
 * Author:    Ajeet Singh Yadav
 * Created:   January 2026
 *
 * GLFW Integration demo: game loop with running flag, event logging,
 * and input polling logs. Stops on ESC or window close.
 * ----------------------------------------------------------------------
 */

namespace vne::events::examples {

class GlfwIntegrationDemo {
   public:
    GlfwIntegrationDemo() = default;
    ~GlfwIntegrationDemo() = default;
    GlfwIntegrationDemo(const GlfwIntegrationDemo&) = delete;
    GlfwIntegrationDemo& operator=(const GlfwIntegrationDemo&) = delete;

    void run();

   private:
    void logInputPolling(int frame);
};

}  // namespace vne::events::examples
