#include <cmath>
#include <vendor/glad/glad.h>
#include "color_shader.hpp"
#include "macros.hpp"
#include "material.hpp"
#include "meshes/cube_test.hpp"
#include "primitives.hpp"
#include "scene.hpp"
#include "scenes/light_scene.hpp"
#include "shader.hpp"
#include <iostream>
#include <ostream>

LightScene::LightScene(Eng::Window *window, Eng::Camera &camera)
    : Eng::Scene(camera) {
  auto object_shader = Eng::ColorShader::create_color_shader(
      shaderpath("object_light.glsl").c_str(),
      EngTypes::Color{1.0f, 0.5f, 0.0f, 1.0f});
  if (!object_shader) {
    std::cout << object_shader.error() << std::endl;
    return;
  }
  box_shader = std::make_unique<Eng::Shader>(std::move(*object_shader));

  auto light_shader = Eng::ColorShader::create_color_shader(
      shaderpath("frag_base_color.glsl").c_str(),
      EngTypes::Color{1.0f, 0.5f, 0.0f, 1.0f});
  if (!light_shader) {
    std::cout << light_shader.error() << std::endl;
    return;
  }
  light_source_shader = std::make_unique<Eng::Shader>(std::move(*light_shader));

  cube_material = std::make_unique<Eng::Material>(*box_shader);
  cube2_material = std::make_unique<Eng::Material>(*box_shader);
  cube3_material = std::make_unique<Eng::Material>(*box_shader);
  cube4_material = std::make_unique<Eng::Material>(*box_shader);
  cube5_material = std::make_unique<Eng::Material>(*box_shader);
  sphere_material = std::make_unique<Eng::Material>(*box_shader);

  cube_mesh = make_cube_with_normals(cube_test_mesh_with_normals);
  sphere_mesh = make_sphere(0.2, 72, 36);

  cube = Eng::RenderObject(&cube_mesh, cube_material.get());
  cube.transform.set_position({-0.8f, 0.2f, -0.1f});
  cube.transform.set_scale({0.6f, 0.6f, 0.6f});
  cube.material->set_diffuse_map(TEXTURES.get(TextureId::MedievalBoxDiffuse));
  cube.material->set_specular_map(TEXTURES.get(TextureId::MedievalBoxSpecular));
  cube.material->set_emission_map(TEXTURES.get(TextureId::Matrix));
  cube.material->set_emission_intensity(2.0f);

  cube2 = Eng::RenderObject(&cube_mesh, cube2_material.get());
  cube2.transform.set_position({0.0f, -0.6f, -1.0f});
  cube2.transform.set_scale({0.35f, 0.35f, 0.35f});

  cube3 = Eng::RenderObject(&cube_mesh, cube3_material.get());
  cube3.transform.set_position({1.0f, 0.5f, -1.5f});
  cube3.transform.set_scale({0.45f, 0.45f, 0.45f});

  cube4 = Eng::RenderObject(&cube_mesh, cube4_material.get());
  cube4.transform.set_position({-1.4f, -0.7f, -2.0f});
  cube4.transform.set_scale({0.3f, 0.7f, 0.3f});

  cube5 = Eng::RenderObject(&cube_mesh, cube5_material.get());
  cube5.transform.set_position({0.7f, -0.8f, 0.2f});
  cube5.transform.set_scale({0.5f, 0.25f, 0.5f});

  sphere = Eng::RenderObject(&sphere_mesh, sphere_material.get());
  sphere.transform.set_position({-0.1f, 0.45f, -1.f});
  sphere.transform.set_scale({1.f, 1.f, 1.f});
  sphere.material->set_diffuse_map(TEXTURES.get(TextureId::Earth));
  sphere.material->set_specular_map(TEXTURES.get(TextureId::Earth));

  light_source_material = std::make_unique<Eng::Material>(*light_source_shader);
  light = Eng::Light(&sphere_mesh, light_source_material.get());
  light.transform.set_position({0.8f, 0.8f, 1.0f});
  light.transform.set_scale({0.15f, 0.15f, 0.15f});
  light.ambient = {0.5f, 0.5f, 0.5f};
  light.diffuse  = {0.5f, 0.5f, 0.5f};
  light.specular = {1.0f, 1.0f, 1.0f};
  light_source_material->patch_uniform("color", light.ambient);

  cube_material->set_light(light);
  cube_material->set_specular({0.6f, 0.6f, 0.6f});
  cube_material->set_ambient({1.0f, 0.1f, 0.1f});
  cube_material->set_diffuse({0.8f, 0.2f, 0.2f});
  cube_material->set_shininess(32.0f);

  cube2_material->set_light(light);
  cube2_material->set_specular({0.6f, 0.6f, 0.6f});
  cube2_material->set_ambient({0.1f, 1.0f, 0.1f});
  cube2_material->set_diffuse({0.2f, 0.8f, 0.2f});
  cube2_material->set_shininess(32.0f);

  cube3_material->set_light(light);
  cube3_material->set_specular({0.6f, 0.6f, 0.6f});
  cube3_material->set_ambient({0.1f, 0.1f, 1.0f});
  cube3_material->set_diffuse({0.2f, 0.2f, 0.8f});
  cube3_material->set_shininess(32.0f);

  cube4_material->set_light(light);
  cube4_material->set_specular({0.6f, 0.6f, 0.6f});
  cube4_material->set_ambient({1.0f, 0.7f, 0.1f});
  cube4_material->set_diffuse({0.9f, 0.5f, 0.1f});
  cube4_material->set_shininess(32.0f);

  cube5_material->set_light(light);
  cube5_material->set_specular({0.6f, 0.6f, 0.6f});
  cube5_material->set_ambient({0.8f, 0.1f, 1.0f});
  cube5_material->set_diffuse({0.6f, 0.2f, 0.9f});
  cube5_material->set_shininess(32.0f); 

  box_shader->patch_uniform("light_pos", light.transform.position);
  box_shader->patch_uniform("view_pos", camera.position);

  objects.push_back(&cube);
  objects.push_back(&cube2);
  objects.push_back(&cube3);
  objects.push_back(&cube4);
  objects.push_back(&cube5);
  objects.push_back(&sphere);
  objects.push_back(&light);
}

void LightScene::update(float delta) {
  float time = glfwGetTime();
  cube.transform.set_rotate(time * 60, {0.4f, 1.0f, 0.0f});
  light.transform.set_rotate(time * 90,{0.4f, 1.0f, 0.0f});

  float speed = 2.0f;
  float radius = 2.5f;

  glm::vec3 light_pos = {
      sinf(time * speed) * radius,
      1.0f,
      cosf(time * speed) * radius
  };

  light_source_material->patch_uniform("color", light.ambient);

  light.transform.set_position(light_pos);
  cube_material->set_light(light);
  cube2_material->set_light(light);
  cube3_material->set_light(light);
  cube4_material->set_light(light);
  cube5_material->set_light(light);
  sphere.transform.set_rotate(time * 60, {0.0f, 1.0f, 0.0f});
  box_shader->patch_uniform("light.position", light_pos);
}