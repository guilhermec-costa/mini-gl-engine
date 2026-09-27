#include "engine.hpp"
#include <GL/gl.h>
#include "GLFW/glfw3.h"
#include "mesh.hpp"
#include "primitives.hpp"
#include <vector>

namespace Eng {
Controller::Controller(Window *window) : _window(window) {};

bool Controller::should_stop() const {
  return glfwWindowShouldClose(_window->unwrap()) || quit_app;
};

void Controller::process_events() {
  if (glfwGetKey(_window->unwrap(), GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    quit_app = true;
  }
}

void Controller::render() {}

void Controller::loop() {
  Eng::Mesh m1(
    std::vector{
      0.0f, 0.5f, 0.0f, 
      0.5f, -0.5f, 0.0f, 
      -0.5f, -0.5f, 0.0f
    },
    std::vector{
      VertexAttribute{0, 3, 0}
    },
    3,
    sizeof(float) * 3
  );

  while (!should_stop()) {
    process_events();
    glClearColor(0.5f, 0.3f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    m1.draw();
    glfwSwapBuffers(_window->unwrap());
    glfwPollEvents();
  }
}
} // namespace Eng