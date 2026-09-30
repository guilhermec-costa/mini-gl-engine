#include "engine.hpp"
#include "GLFW/glfw3.h"
#include "albedo.hpp"
#include "color_shader.hpp"
#include "glm/ext/matrix_transform.hpp"
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

  auto color_shader = ColorShader::create_color_shader(
    shaderpath("frag_base_color.glsl").c_str(), 
    EngTypes::Color{1.0f, 0.5f, 0.0f, 1.0f}
  );

  if(!color_shader) {
    std::cout << color_shader.error() << std::endl;
    return;
  }
  glm::mat4 model = glm::mat4(1.0f);
  model = glm::translate(model, glm::vec3(0.3f, 0.0f, 0.0f));
  color_shader->patch_uniform("model", model);

  Albedo albedo(albedopath("wood.jpg").c_str(), GL_RGB, GL_RGB);
  Material material(*color_shader, albedo);

  Eng::Mesh m1(
    std::vector{
      0.0f, 0.5f, 0.0f, 0.5f, 1.0f,
      0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
      -0.5f, -0.5f, 0.0f, 1.0f, 0.0f
    },
    std::vector{
      VertexAttribute(0, 3, 0),
      VertexAttribute(1, 2, sizeof(float) * 3)
    },
    3,
    sizeof(float) * 5,
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