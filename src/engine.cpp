#include "engine.hpp"
#include <GL/gl.h>
#include "GLFW/glfw3.h"

namespace Eng {
Controller::Controller(Window *window) : _window(window) {};

bool Controller::should_stop() const {
  return glfwWindowShouldClose(_window->unwrap());
};

void Controller::loop() {
  while(!should_stop()) {
    glClearColor(0.5f, 0.3f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(_window->unwrap());
    glfwPollEvents();
  }
}
} // namespace Eng