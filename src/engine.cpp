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
Controller::Controller(Window *window, Renderer* renderer) 
  : _window(window), _renderer(renderer) {};

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
  auto texture_shader = Shader::create(shaderpath("vertex.glsl").c_str(), shaderpath("frag.glsl").c_str());
  if(!texture_shader) {
    std::cout << texture_shader.error() << std::endl;
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

  Albedo albedo(albedopath("wood.jpg").c_str(), GL_RGB, GL_RGB);
  Material texture_material(*texture_shader, albedo);
  Material color_material(*color_shader);

  Eng::Mesh t1(
    std::vector{
      0.65f,  0.5f, 0.0f,   0.5f, 1.0f,
      0.95f, -0.5f, 0.0f,   0.0f, 0.0f,
      0.35f, -0.5f, 0.0f,   1.0f, 0.0f
    },
    std::vector{
      VertexAttribute(0, 3, 0),
      VertexAttribute(1, 2, sizeof(float) * 3)
    },
    3,
    sizeof(float) * 5,
    &texture_material
  );

  Eng::Mesh t2(
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
    &color_material
  );
  
  while (!should_stop()) {
    process_events();
    _window->clear({0.0f, 0.0f, 0.0f, 1.0f});

    texture_shader->patch_uniform("albedo", 0);
    _renderer->draw(t1);
    color_shader->patch_uniform("color", EngTypes::Color(0.f, 0.f, 255.f, 1.f));
    _renderer->draw(t2);
    glfwSwapBuffers(_window->unwrap());
    glfwPollEvents();
  }
}
} // namespace Eng