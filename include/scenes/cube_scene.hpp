#pragma once

#include "material.hpp"
#include "scene.hpp"
#include "texture2d.hpp"
#include "window.hpp"

class CubeScene : public Eng::Scene {
public:
  CubeScene(Eng::Window * window, Eng::Camera& camera);
  void update(float delta) override;

private:
  std::unique_ptr<Eng::Shader> color_shader;
  std::unique_ptr<Eng::Shader> texture_shader;
  Eng::Texture2D texture;
  std::unique_ptr<Eng::Material> cube_material;
  std::unique_ptr<Eng::Material> triangle_material;

  Eng::Mesh cube_mesh;
  Eng::Mesh triangle_mesh;
  Eng::RenderObject cube;
  Eng::RenderObject triangle;
};