#pragma once

#include "material.hpp"
#include "scene.hpp"
#include "texture2d.hpp"
#include "window.hpp"

class LightScene : public Eng::Scene {
public:
  LightScene(Eng::Window * window, Eng::Camera& camera);
  void update(float delta) override;
  void prepare_render() override;

private:
  std::unique_ptr<Eng::Shader> light_source_shader;
  std::unique_ptr<Eng::Shader> box_shader;
  Eng::Texture2D wood_texture;
  Eng::Texture2D wall_texture;
  Eng::Mesh cube_mesh;
  Eng::Mesh sphere_mesh;

  // Materials
  std::unique_ptr<Eng::Material> cube_material;
  std::unique_ptr<Eng::Material> cube2_material;
  std::unique_ptr<Eng::Material> cube3_material;
  std::unique_ptr<Eng::Material> cube4_material;
  std::unique_ptr<Eng::Material> cube5_material;
  std::unique_ptr<Eng::Material> sphere_material;
  std::unique_ptr<Eng::Material> light_source_material;

  // Objects
  Eng::RenderObject cube;
  Eng::RenderObject cube2;
  Eng::RenderObject cube3;
  Eng::RenderObject cube4;
  Eng::RenderObject cube5;
  Eng::RenderObject sphere;
  Eng::RenderObject light_source;
};