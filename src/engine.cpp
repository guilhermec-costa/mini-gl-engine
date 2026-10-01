#include "engine.hpp"
#include "GLFW/glfw3.h"
#include "renderer.hpp"
#include "texture2d.hpp"
#include "color_shader.hpp"
#include "material.hpp"
#include "mesh.hpp"
#include "primitives.hpp"
#include "shader.hpp"
#include "meshes/cube_test.hpp"
#include <iostream>

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

constexpr float CAMERA_FOV = 45.0f;

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

  Texture2D albedo(albedopath("wood.jpg").c_str(), GL_RGB, GL_RGB);
  Material texture_material(*texture_shader, albedo);
  Material color_material(*color_shader);

  Eng::Mesh tri1mesh = make_triangle(
      glm::vec3(-0.8f,  0.5f, 0.0f),
      glm::vec3(-0.3f, -0.5f, 0.0f),
      glm::vec3(-1.0f, -0.5f, 0.0f),
      &texture_material
  );
  RenderObject tri1{tri1mesh};

  Eng::Mesh tri2mesh = make_triangle(
      glm::vec3( 0.8f,  0.5f, 0.0f),
      glm::vec3( 1.0f, -0.5f, 0.0f),
      glm::vec3( 0.3f, -0.5f, 0.0f),
      &color_material
  );
  RenderObject tri2{tri2mesh};

  Eng::Mesh quadmesh = make_quad(
    glm::vec3(-0.5f,  0.5f, 0.0f),
    glm::vec3( 0.5f,  0.5f, 0.0f),
    glm::vec3( 0.5f, -0.5f, 0.0f),
    glm::vec3(-0.5f, -0.5f, 0.0f),
    &color_material
  );
  RenderObject quad{quadmesh};

  Eng::Mesh cubemesh = make_cube(cube_mesh, &texture_material);
  RenderObject cube{cubemesh};
  
  glEnable(GL_DEPTH_TEST);

  float delta_time = 0.0f, last_frame_time = 0.0f;
  while (!should_stop()) {
    const float frame_time = glfwGetTime();
    delta_time = frame_time - last_frame_time;
    last_frame_time = frame_time;

    process_events();
    _window->clear({0.0f, 0.0f, 0.0f, 1.0f});

    texture_shader->patch_uniform("albedo", 0);
    color_shader->patch_uniform("color", EngTypes::Color(128.0f, 128.0f, 0.0f, 1.0f));

    tri1.transform.set_position(glm::vec3(-0.8f,  0.5f, 0.0f));
    tri1.transform.set_scale(glm::vec3(0.3f, 0.3f, 1.0f));
    tri1.transform.set_rotate(glfwGetTime() * 60, glm::vec3(1.0f, 0.0, 1.0f));
    _renderer->draw(tri1);
    color_shader->patch_uniform("color", EngTypes::Color(128.0f, 128.0f, 128.0f, 1.0f));
    _renderer->draw(tri2);
    color_shader->patch_uniform("color", EngTypes::Color{1.0f, 0.0, 125.0f, 1.0f});

    cube.transform.set_rotate(glfwGetTime() * 60, glm::vec3(0.0f, 1.0f, 0.0));
    _renderer->draw(cube);
    glfwSwapBuffers(_window->unwrap());
    glfwPollEvents();
  }
}
} // namespace Eng