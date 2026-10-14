#include "scenes/model_playground.hpp"

#include "macros.hpp"
#include "resource/shader_storage.hpp"

using namespace Engine;

ModelPlayground::ModelPlayground(Window* window, Camera& camera)
    : Scene(camera),
      backpack(
          "/home/guichina/dev/mini-gl-engine/assets/models/backpack/"
          "backpack.obj",
          *SHADERS.get(ShaderId::Base)),
      door("/home/guichina/dev/mini-gl-engine/assets/models/door/door2.glb",
           *SHADERS.get(ShaderId::Base)) {
  Shader* shader = SHADERS.get(ShaderId::Base);

  shader->patch_uniform("use_directional_light", 1);
  shader->patch_uniform("use_point_light", 0);
  shader->patch_uniform("use_spotlight", 0);

  shader->patch_uniform("directional_light.direction", glm::vec3{-0.2f, -1.0f, -0.3f});

  shader->patch_uniform("directional_light.ambient", glm::vec3{0.2f, 0.2f, 0.2f});

  shader->patch_uniform("directional_light.diffuse", glm::vec3{1.0f, 1.0f, 1.0f});

  shader->patch_uniform("directional_light.specular", glm::vec3{1.0f, 1.0f, 1.0f});

  for (Mesh& mesh : backpack.get_meshes()) {
    auto* obj = new RenderObject(&mesh);
    obj->transform.set_position({-0.8f, 0.2f, -0.1f});
    objects.push_back(obj);
  }

  for (Mesh& mesh : door.get_meshes()) {
    auto* obj = new RenderObject(&mesh);
    obj->transform.set_position({3.f, 0.2f, -0.1f});
    objects.push_back(obj);
  }
}

void ModelPlayground::update(float delta) {}