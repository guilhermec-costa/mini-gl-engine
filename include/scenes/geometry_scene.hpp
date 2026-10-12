#pragma once

#include "material.hpp"
#include "mesh.hpp"
#include "scene.hpp"
#include "shader.hpp"
#include "window.hpp"

class GeometryScene : public Engine::Scene {
public:
  GeometryScene(Engine::Window *window, Engine::Camera &camera);
  void update(float delta) override;

private:
  std::unique_ptr<Engine::Shader> color_shader;

  std::unique_ptr<Engine::Material> red_material;
  std::unique_ptr<Engine::Material> green_material;
  std::unique_ptr<Engine::Material> blue_material;
  std::unique_ptr<Engine::Material> yellow_material;

  // meshes
  Engine::Mesh triangle_mesh;
  Engine::Mesh quad_mesh;

  // triangles
  Engine::RenderObject triangle_left;
  Engine::RenderObject triangle_center;
  Engine::RenderObject triangle_right;

  // quads
  Engine::RenderObject quad_left;
  Engine::RenderObject quad_right;
};