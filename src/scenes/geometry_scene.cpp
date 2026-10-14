#include "scenes/geometry_scene.hpp"

#include <GLFW/glfw3.h>

#include <iostream>

#include "color_shader.hpp"
#include "primitives.hpp"

GeometryScene::GeometryScene(Engine::Window* window, Engine::Camera& camera)
    : Engine::Scene(camera) {
  auto shader_result = Engine::ColorShader::create_color_shader(
      shaderpath("frag_base_color.glsl").c_str(), EngineTypes::Color{0.0f, 0.0f, 0.0f, 1.0f});

  if (!shader_result) {
    std::cout << shader_result.error() << '\n';
    return;
  }

  color_shader = std::make_unique<Engine::Shader>(std::move(*shader_result));

  red_material = std::make_unique<Engine::Material>(*color_shader);
  green_material = std::make_unique<Engine::Material>(*color_shader);
  blue_material = std::make_unique<Engine::Material>(*color_shader);
  yellow_material = std::make_unique<Engine::Material>(*color_shader);

  red_material->patch_uniform("color", EngineTypes::Color{255.0f, 40.0f, 40.0f, 255.0f});

  green_material->patch_uniform("color", EngineTypes::Color{40.0f, 255.0f, 80.0f, 255.0f});

  blue_material->patch_uniform("color", EngineTypes::Color{40.0f, 100.0f, 255.0f, 255.0f});

  yellow_material->patch_uniform("color", EngineTypes::Color{255.0f, 210.0f, 40.0f, 255.0f});

  // --------------------------------------------------
  // Meshes
  // --------------------------------------------------

  triangle_mesh = make_triangle(glm::vec3{-0.5f, -0.5f, 0.0f}, glm::vec3{0.5f, -0.5f, 0.0f},
                                glm::vec3{0.0f, 0.5f, 0.0f});

  quad_mesh = make_quad(glm::vec3{-0.5f, 0.5f, 0.0f}, glm::vec3{0.5f, 0.5f, 0.0f},
                        glm::vec3{0.5f, -0.5f, 0.0f}, glm::vec3{-0.5f, -0.5f, 0.0f});

  triangle_left = Engine::RenderObject(&triangle_mesh, red_material.get());
  triangle_center = Engine::RenderObject(&triangle_mesh, green_material.get());
  triangle_right = Engine::RenderObject(&triangle_mesh, blue_material.get());

  triangle_left.transform.set_position({-1.5f, 0.8f, 0.0f});
  triangle_center.transform.set_position({0.0f, 0.8f, 0.0f});
  triangle_right.transform.set_position({1.5f, 0.8f, 0.0f});

  quad_left = Engine::RenderObject(&quad_mesh, blue_material.get());
  quad_right = Engine::RenderObject(&quad_mesh, yellow_material.get());
  quad_left.transform.set_position({-0.9f, -0.8f, 0.0f});
  quad_right.transform.set_position({0.9f, -0.8f, 0.0f});

  objects.push_back(&triangle_left);
  objects.push_back(&triangle_center);
  objects.push_back(&triangle_right);
  objects.push_back(&quad_left);
  objects.push_back(&quad_right);
}

void GeometryScene::update(float delta) {
  const float time = static_cast<float>(glfwGetTime());

  triangle_left.transform.set_rotate(time * 40.0f, {0.0f, 0.0f, 1.0f});
  triangle_center.transform.set_rotate(time * -70.0f, {0.0f, 0.0f, 1.0f});
  triangle_right.transform.set_rotate(time * 100.0f, {0.0f, 0.0f, 1.0f});
  quad_left.transform.set_rotate(time * 50.0f, {1.0f, 0.3f, 0.0f});
  quad_right.transform.set_rotate(time * -60.0f, {0.3f, 1.0f, 0.0f});
}