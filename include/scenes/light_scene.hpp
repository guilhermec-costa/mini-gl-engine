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
  std::unique_ptr<Eng::Shader> texture_shader;
  std::unique_ptr<Eng::Shader> color_shader;
  Eng::Texture2D wood_texture;
  Eng::Texture2D wall_texture;
  std::unique_ptr<Eng::Material> cube_material;
  std::unique_ptr<Eng::Material> light_source_material;

  Eng::Mesh cube_mesh;
  Eng::RenderObject cube;
  Eng::RenderObject light_source;
};