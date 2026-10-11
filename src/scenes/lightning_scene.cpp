#include <cmath>
#include <vendor/glad/glad.h>
#include "color_shader.hpp"
#include "light.hpp"
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
  lamp_shader = std::make_unique<Eng::Shader>(std::move(*light_shader));

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

  lamp_material = std::make_unique<Eng::Material>(*lamp_shader);
  lamp = Eng::RenderObject{&sphere_mesh, lamp_material.get()};
  lamp.transform.set_position({0.8f, 0.8f, 1.0f});
  lamp.transform.set_scale({0.15f, 0.15f, 0.15f});

  point_light = Eng::PointLight();
  point_light.position = lamp.transform.position;
  point_light.radius = 6.0f;

  point_light.ambient  = {0.05f, 0.05f, 0.05f};
  point_light.diffuse  = {0.8f, 0.8f, 0.8f};
  point_light.specular = {1.0f, 1.0f, 1.0f};

  point_light.constant  = 1.0f;
  point_light.linear    = 0.09f;
  point_light.quadratic = 0.032f;

  directional_light = Eng::DirectionalLight();
  directional_light.direction = {-0.2f, -1.0f, -0.3f};
  directional_light.ambient  = {0.08f, 0.015f, 0.015f};
  directional_light.diffuse  = {0.7f, 0.15f, 0.12f};
  directional_light.specular = {0.8f, 0.25f, 0.2f};

  spotlight = Eng::Spotlight();
  spotlight.position = camera.position;
  spotlight.direction = camera.front;

  spotlight.ambient  = {0.0f, 0.0f, 0.0f};
  spotlight.diffuse  = {1.0f, 1.0f, 1.0f};
  spotlight.specular = {1.0f, 1.0f, 1.0f};

  spotlight.constant  = 1.0f;
  spotlight.linear    = 0.09f;
  spotlight.quadratic = 0.032f;
  spotlight.radius    = 6.0f;

  spotlight.cut_off = glm::cos(glm::radians(12.5f));
  spotlight.outer_cut_off = glm::cos(glm::radians(20.5));

  lamp_material->patch_uniform("color", point_light.diffuse);

  cube_material->set_specular({0.6f, 0.6f, 0.6f});
  cube_material->set_ambient({1.0f, 0.1f, 0.1f});
  cube_material->set_diffuse({0.8f, 0.2f, 0.2f});
  cube_material->set_shininess(32.0f);

  cube2_material->set_specular({0.6f, 0.6f, 0.6f});
  cube2_material->set_ambient({0.1f, 1.0f, 0.1f});
  cube2_material->set_diffuse({0.2f, 0.8f, 0.2f});
  cube2_material->set_shininess(32.0f);

  cube3_material->set_specular({0.6f, 0.6f, 0.6f});
  cube3_material->set_ambient({0.1f, 0.1f, 1.0f});
  cube3_material->set_diffuse({0.2f, 0.2f, 0.8f});
  cube3_material->set_shininess(32.0f);

  cube4_material->set_specular({0.6f, 0.6f, 0.6f});
  cube4_material->set_ambient({1.0f, 0.7f, 0.1f});
  cube4_material->set_diffuse({0.9f, 0.5f, 0.1f});
  cube4_material->set_shininess(32.0f);

  cube5_material->set_specular({0.6f, 0.6f, 0.6f});
  cube5_material->set_ambient({0.8f, 0.1f, 1.0f});
  cube5_material->set_diffuse({0.6f, 0.2f, 0.9f});
  cube5_material->set_shininess(32.0f); 

  box_shader->patch_uniform("view_pos", camera.position);

  objects.push_back(&cube);
  objects.push_back(&cube2);
  objects.push_back(&cube3);
  objects.push_back(&cube4);
  objects.push_back(&cube5);
  objects.push_back(&sphere);
  objects.push_back(&lamp);
}

void LightScene::update(float delta) {
  float time = glfwGetTime();
  cube.transform.set_rotate(time * 60, {0.4f, 1.0f, 0.0f});

  float speed = 2.0f;
  float radius = 2.5f;

  glm::vec3 light_pos = {
      sinf(time * speed) * radius,
      1.0f,
      cosf(time * speed) * radius
  };

  lamp_material->patch_uniform("color", point_light.diffuse);
  lamp.transform.set_position(light_pos);
  point_light.position = light_pos;
  cube_material->set_point_light(point_light);
  cube2_material->set_point_light(point_light);
  cube3_material->set_point_light(point_light);
  cube4_material->set_point_light(point_light);
  cube5_material->set_point_light(point_light);
  cube_material->set_directional_light(directional_light);
  cube2_material->set_directional_light(directional_light);
  cube3_material->set_directional_light(directional_light);
  cube4_material->set_directional_light(directional_light);
  cube5_material->set_directional_light(directional_light);
  sphere.transform.set_rotate(time * 60, {0.0f, 1.0f, 0.0f});

  spotlight.position = camera.position;
  spotlight.direction = camera.front;

  cube_material->set_spotlight(spotlight);
  cube2_material->set_spotlight(spotlight);
  cube3_material->set_spotlight(spotlight);
  cube4_material->set_spotlight(spotlight);
  cube5_material->set_spotlight(spotlight);
  sphere_material->set_spotlight(spotlight);
}