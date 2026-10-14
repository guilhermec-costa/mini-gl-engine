#include "scenes/solar_system.hpp"

#include "macros.hpp"
#include "material.hpp"
#include "primitives.hpp"
#include "render_object.hpp"
#include "window.hpp"

using namespace Engine;

SolarSystem::SolarSystem(Window* window, Camera& camera) : Scene(camera) {
  base_shader = SHADERS.get(ShaderId::Base);
  sun_material = std::make_unique<Engine::Material>(*base_shader);
  sun_mesh = make_sphere(1, 72, 36);
  sun = RenderObject{&sun_mesh, sun_material.get()};
  sun.transform.set_position({-0.1f, 0.45f, -1.f});
  sun.transform.set_scale({1.f, 1.f, 1.f});

  auto* sun_texture = TEXTURES.get(texturepath("sun.jpg"));
  sun.material->set_diffuse_map(sun_texture);
  sun_material->set_diffuse_map(sun_texture);
  sun_material->set_emission_map(sun_texture);
  sun_material->set_emission_intensity(1.5f);

  directional_light = Engine::DirectionalLight();
  directional_light.direction = {0.2f, -1.0f, 0.3f};
  directional_light.ambient = {1.0f, 1.0f, 1.0f};
  directional_light.diffuse = {1.0f, 1.0f, 1.0f};
  directional_light.specular = {0.8f, 0.25f, 0.2f};

  objects.push_back(&sun);
};

void SolarSystem::update(float delta) {}