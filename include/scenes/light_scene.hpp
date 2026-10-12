#pragma once

#include "light.hpp"
#include "material.hpp"
#include "scene.hpp"
#include "texture2d.hpp"
#include "window.hpp"

class LightScene : public Engine::Scene {
public:
  LightScene(Engine::Window * window, Engine::Camera& camera);
  void update(float delta) override;

private:
  std::unique_ptr<Engine::Shader> lamp_shader;
  std::unique_ptr<Engine::Shader> box_shader;
  Engine::Texture2D wood_texture;
  Engine::Texture2D wall_texture;
  Engine::Mesh cube_mesh;
  Engine::Mesh sphere_mesh;

  // Materials
  std::unique_ptr<Engine::Material> cube_material;
  std::unique_ptr<Engine::Material> cube2_material;
  std::unique_ptr<Engine::Material> cube3_material;
  std::unique_ptr<Engine::Material> cube4_material;
  std::unique_ptr<Engine::Material> cube5_material;
  std::unique_ptr<Engine::Material> sphere_material;
  std::unique_ptr<Engine::Material> lamp_material;

  // Objects
  Engine::RenderObject cube;
  Engine::RenderObject cube2;
  Engine::RenderObject cube3;
  Engine::RenderObject cube4;
  Engine::RenderObject cube5;
  Engine::RenderObject sphere;
  Engine::RenderObject lamp;
  Engine::PointLight point_light;
  Engine::DirectionalLight directional_light;
  Engine::Spotlight spotlight;
};