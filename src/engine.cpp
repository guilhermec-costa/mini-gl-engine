#include "engine.hpp"
#include "GLFW/glfw3.h"
#include "material.hpp"
#include "mesh.hpp"
#include "primitives.hpp"
#include "shader.hpp"
#include <iostream>
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
  auto shader = Shader::create(shaderpath("vertex.glsl").c_str(), shaderpath("frag.glsl").c_str());
  if(!shader) {
    std::cout << shader.error() << std::endl;
    return;
  }
  Material material(*shader);

  Eng::Mesh m1(
    std::vector{
      0.0f, 0.5f, 0.0f, 
      0.5f, -0.5f, 0.0f, 
      -0.5f, -0.5f, 0.0f
    },
    std::vector{
      VertexAttribute(0, 3, 0)
    },
    3,
    sizeof(float) * 3,
    &material
  );
  
  while (!should_stop()) {
    process_events();
    _window->clear({0.0f, 0.0f, 0.0f, 1.0f});
    m1.draw();
    glfwSwapBuffers(_window->unwrap());
    glfwPollEvents();
  }
}
} // namespace Eng