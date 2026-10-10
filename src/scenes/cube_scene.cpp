#include <vendor/glad/glad.h>
#include "scenes/cube_scene.hpp"
#include "GLFW/glfw3.h"
#include "color_shader.hpp"
#include "material.hpp"
#include "meshes/cube_test.hpp"
#include "primitives.hpp"
#include "scene.hpp"
#include "shader.hpp"
#include "texture2d.hpp"
#include <iostream>

CubeScene::CubeScene(Eng::Window* window, Eng::Camera& camera) : Eng::Scene(camera) {
  auto _texture_shader = Eng::Shader::create(shaderpath("vertex.glsl").c_str(), shaderpath("frag.glsl").c_str());
  if(!_texture_shader) {
    std::cout << _texture_shader.error() << std::endl;
    return;
  }
  texture_shader = std::make_unique<Eng::Shader>(std::move(*_texture_shader));
  texture = Eng::Texture2D(albedopath("wood.jpg").c_str());
  cube_material = std::make_unique<Eng::Material>(*texture_shader);
  cube_mesh = make_cube(cube_test_mesh);
  cube = Eng::RenderObject(&cube_mesh, cube_material.get());

  auto _color_shader = Eng::ColorShader::create_color_shader(
      shaderpath("frag_base_color.glsl").c_str(),
      EngTypes::Color{1.0f, 0.5f, 0.0f, 1.0f});

  if (!_color_shader) {
    std::cout << _color_shader.error() << std::endl;
    return;
  }
  color_shader = std::make_unique<Eng::Shader>(std::move(*_color_shader));
  triangle_material = std::make_unique<Eng::Material>(*color_shader);
  triangle_mesh = make_triangle(
      glm::vec3(0.8f,  0.5f, -0.8f),
      glm::vec3(1.0f, -0.5f, -0.8f),
      glm::vec3(0.3f, -0.5f, -0.8f)
  );
  triangle = Eng::RenderObject(&triangle_mesh, triangle_material.get());

  objects.push_back(&triangle);
  objects.push_back(&cube);
}

void CubeScene::update(float delta) {
  cube.transform.set_rotate(glfwGetTime() * 60, {0.4f, 1.0f, 0.0});
  triangle.transform.set_scale({1.2f, 1.2f, 1.02f});
  triangle.transform.set_rotate(glfwGetTime() * 90, {0.0f, 0.5f, 1.0f});
}