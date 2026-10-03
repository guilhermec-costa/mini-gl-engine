#pragma once

#include "material.hpp"
#include "mesh.hpp"
#include "scene.hpp"
#include "shader.hpp"
#include "window.hpp"

class GeometryScene : public Eng::Scene {
public:
  GeometryScene(Eng::Window *window, Eng::Camera &camera);

  void prepare_render() override;
  void update(float delta) override;

private:
  std::unique_ptr<Eng::Shader> color_shader;

  std::unique_ptr<Eng::Material> red_material;
  std::unique_ptr<Eng::Material> green_material;
  std::unique_ptr<Eng::Material> blue_material;
  std::unique_ptr<Eng::Material> yellow_material;

  // meshes
  Eng::Mesh triangle_mesh;
  Eng::Mesh quad_mesh;

  // triangles
  Eng::RenderObject triangle_left;
  Eng::RenderObject triangle_center;
  Eng::RenderObject triangle_right;

  // quads
  Eng::RenderObject quad_left;
  Eng::RenderObject quad_right;
};