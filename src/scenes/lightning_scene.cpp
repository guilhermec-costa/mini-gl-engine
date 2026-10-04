#include <glad/glad.h>
#include "scenes/light_scene.hpp"
#include "color_shader.hpp"
#include "material.hpp"
#include "meshes/cube_test.hpp"
#include "primitives.hpp"
#include "scene.hpp"
#include "shader.hpp"
#include "texture2d.hpp"
#include <iostream>

LightScene::LightScene(Eng::Window* window, Eng::Camera& camera) : Eng::Scene(camera) {
  auto _texture_shader = Eng::Shader::create(shaderpath("vertex.glsl").c_str(), shaderpath("frag.glsl").c_str());
  if(!_texture_shader) {
    std::cout << _texture_shader.error() << std::endl;
    return;
  }
  texture_shader = std::make_unique<Eng::Shader>(std::move(*_texture_shader));

  auto _color_shader = Eng::ColorShader::create_color_shader(
      shaderpath("frag_base_color.glsl").c_str(),
      EngTypes::Color{1.0f, 0.5f, 0.0f, 1.0f});
  if (!_color_shader) {
    std::cout << _color_shader.error() << std::endl;
    return;
  }
  color_shader = std::make_unique<Eng::Shader>(std::move(*_color_shader));

  wall_texture = Eng::Texture2D(albedopath("wall.jpg").c_str(), GL_RGB, GL_RGB);
  cube_material = std::make_unique<Eng::Material>(*color_shader);
  cube_mesh = make_cube(cube_test_mesh);
  cube = Eng::RenderObject(&cube_mesh, cube_material.get());
  cube.transform.set_position({-0.8f, 0.2f, -0.1f});
  cube.transform.set_scale({0.6f, 0.6f, 0.6f});

  light_source_material = std::make_unique<Eng::Material>(*color_shader);
  light_source = Eng::RenderObject(&cube_mesh, light_source_material.get());
  light_source.transform.set_position({0.6f, 0.3f, 1.0f});
  light_source.transform.set_scale({0.15f, 0.15f, 0.15f});

  glm::vec3 light_color(1.0f, 1.0f, 1.0f);
  glm::vec3 box_color(1.0f, 0.5f, 0.31f);
  glm::vec3 box_reflected_light = light_color * box_color;
  light_source_material->patch_uniform("color", glm::vec4{light_color, 1.0f});
  cube_material->patch_uniform("color", glm::vec4{box_reflected_light, 1.0f});
  objects.push_back(&cube);
  objects.push_back(&light_source);
}

void LightScene::prepare_render() {
}

void LightScene::update(float delta) {
  cube.transform.set_rotate(glfwGetTime() * 60, {0.4f, 1.0f, 0.0f});
}