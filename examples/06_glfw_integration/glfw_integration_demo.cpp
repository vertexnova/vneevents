/* ---------------------------------------------------------------------
 * Copyright (c) 2026 Ajeet Singh Yadav. All rights reserved.
 * Licensed under the Apache License, Version 2.0 (the "License")
 *
 * Author:    Ajeet Singh Yadav
 * Created:   January 2026
 *
 * GLFW Integration demo: running flag, event logs, input polling logs
 * ----------------------------------------------------------------------
 */

#include "glfw_integration_demo.h"
#include "glfw_integration.h"

#include <vertexnova/events/events.h>
#include <vertexnova/logging/logging.h>

#include <glad/glad.h>

namespace {
CREATE_VNE_LOGGER_CATEGORY("vneevents.examples.glfw_integration")
}

namespace vne::events::examples {

namespace {

class GlfwEventLogger : public vne::events::EventListener {
   public:
    explicit GlfwEventLogger(bool* running) : running_(running) {}
    void onEvent(const vne::events::Event& event) override {
        using vne::events::EventType;
        switch (event.type()) {
            case EventType::eWindowClose:
                VNE_LOG_INFO << "  [Event] Window close requested";
                *running_ = false;
                break;
            case EventType::eWindowResize: {
                const auto& e = static_cast<const vne::events::WindowResizeEvent&>(event);
                VNE_LOG_INFO << "  [Event] Window resized: " << e.width() << "x" << e.height();
                break;
            }
            case EventType::eKeyPressed: {
                const auto& e = static_cast<const vne::events::KeyPressedEvent&>(event);
                VNE_LOG_INFO << "  [Event] Key pressed: " << static_cast<int>(e.keyCode());
                if (e.keyCode() == vne::events::KeyCode::eEscape) *running_ = false;
                break;
            }
            case EventType::eKeyReleased: {
                const auto& e = static_cast<const vne::events::KeyReleasedEvent&>(event);
                VNE_LOG_INFO << "  [Event] Key released: " << static_cast<int>(e.keyCode());
                break;
            }
            case EventType::eMouseMoved: {
                const auto& e = static_cast<const vne::events::MouseMovedEvent&>(event);
                VNE_LOG_INFO << "  [Event] Mouse moved: (" << e.x() << ", " << e.y() << ")";
                break;
            }
            case EventType::eMouseButtonPressed: {
                const auto& e = static_cast<const vne::events::MouseButtonPressedEvent&>(event);
                VNE_LOG_INFO << "  [Event] Mouse button pressed: " << static_cast<int>(e.button());
                break;
            }
            case EventType::eMouseButtonReleased: {
                const auto& e = static_cast<const vne::events::MouseButtonReleasedEvent&>(event);
                VNE_LOG_INFO << "  [Event] Mouse button released: " << static_cast<int>(e.button());
                break;
            }
            default:
                break;
        }
    }

   private:
    bool* running_;
};

}  // namespace

void GlfwIntegrationDemo::run() {
    VNE_LOG_INFO << "=== GLFW Integration Demo ===";
    VNE_LOG_INFO << "";

    auto& manager = vne::events::EventManager::instance();
    auto glfw = std::make_unique<GLFWIntegration>(manager);

    if (!glfw->initialize(800, 600, "VneEvents - GLFW Integration")) {
        VNE_LOG_ERROR << "Failed to initialize GLFW window.";
        return;
    }

    VNE_LOG_INFO << "  Window open. Use keys (W,A,S,D,Space), mouse, resize; ESC or close to exit.";
    VNE_LOG_INFO << "";

    bool running = true;
    auto logger = std::make_shared<GlfwEventLogger>(&running);

    using ET = vne::events::EventType;
    manager.registerListener(ET::eWindowClose, logger);
    manager.registerListener(ET::eWindowResize, logger);
    manager.registerListener(ET::eKeyPressed, logger);
    manager.registerListener(ET::eKeyReleased, logger);
    manager.registerListener(ET::eMouseMoved, logger);
    manager.registerListener(ET::eMouseButtonPressed, logger);
    manager.registerListener(ET::eMouseButtonReleased, logger);

    int frame = 0;
    while (running && !glfw->shouldClose()) {
        glfw->pollEvents();
        manager.processEvents();

        logInputPolling(frame);

        glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glfw->swapBuffers();

        vne::events::Input::nextFrame();
        ++frame;
    }

    glfw->shutdown();
    VNE_LOG_INFO << "";
    VNE_LOG_INFO << "=== Demo finished ===";
}

void GlfwIntegrationDemo::logInputPolling(int frame) {
    using vne::events::Input;
    using vne::events::KeyCode;
    using vne::events::MouseButton;

    int w = Input::isKeyPressed(static_cast<int>(KeyCode::eW)) ? 1 : 0;
    int a = Input::isKeyPressed(static_cast<int>(KeyCode::eA)) ? 1 : 0;
    int s = Input::isKeyPressed(static_cast<int>(KeyCode::eS)) ? 1 : 0;
    int d = Input::isKeyPressed(static_cast<int>(KeyCode::eD)) ? 1 : 0;
    int sp = Input::isKeyPressed(static_cast<int>(KeyCode::eSpace)) ? 1 : 0;
    auto [mx, my] = Input::mousePosition();
    int lmb = Input::isMouseButtonPressed(static_cast<int>(MouseButton::eLeft)) ? 1 : 0;

    // Log every frame; optionally throttle by logging only when input active or every 60 frames
    bool any_active = (w | a | s | d | sp | lmb) != 0;
    if (any_active || (frame % 60 == 0)) {
        VNE_LOG_INFO << "  [Input poll] W=" << w << " A=" << a << " S=" << s << " D=" << d
                     << " Space=" << sp << " mouse=(" << mx << "," << my << ") LMB=" << lmb;
    }
}

}  // namespace vne::events::examples
