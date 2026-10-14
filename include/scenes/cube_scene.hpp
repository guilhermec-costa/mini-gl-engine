#pragma once

#include "material.hpp"
#include "scene.hpp"
#include "texture2d.hpp"
#include "window.hpp"

class CubeScene : public Engine::Scene {
 public:
  CubeScene(Engine::Window* window, Engine::Camera& camera);
  void update(float delta) override;

 private:
  std::unique_ptr<Engine::Shader> color_shader;
  std::unique_ptr<Engine::Shader> texture_shader;
  Engine::Texture2D texture;
  std::unique_ptr<Engine::Material> cube_material;
  std::unique_ptr<Engine::Material> triangle_material;

  Engine::Mesh cube_mesh;
  Engine::Mesh triangle_mesh;
  Engine::RenderObject cube;
  Engine::RenderObject triangle;
};