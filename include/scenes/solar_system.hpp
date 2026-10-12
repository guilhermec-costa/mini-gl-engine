#pragma once

#include "material.hpp"
#include "mesh.hpp"
#include "render_object.hpp"
#include "scene.hpp"
#include "window.hpp"
#include <memory>

class SolarSystem : public Engine::Scene {
public:
  SolarSystem(Engine::Window *window, Engine::Camera &camera);
  void update(float delta) override;

private:
  Engine::Shader* base_shader;
  std::unique_ptr<Engine::Material> sun_material;
  Engine::Mesh sun_mesh;
  Engine::RenderObject sun;
  Engine::DirectionalLight directional_light;
};