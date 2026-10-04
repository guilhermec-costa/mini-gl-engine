#include <glad/glad.h>
#include <iostream>
#include "renderer.hpp"

namespace Eng {

Renderer::Renderer() {
  glEnable(GL_DEPTH_TEST);
  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void Renderer::draw(const RenderObject& object, const Camera& camera) {
  Material* material = object.material;
  if(material) {
    material->shader.patch_uniform("model", object.transform.matrix());
    material->shader.patch_uniform("view", camera.view_matrix());
    material->shader.patch_uniform("projection", camera.projection_matrix());
    material->bind();
  }

  glBindVertexArray(object.mesh->get_VAO());
  if(object.mesh->get_index_count() > 0) {
    glDrawElements(GL_TRIANGLES, object.mesh->get_index_count(), GL_UNSIGNED_INT, nullptr);
  } else {
    glDrawArrays(GL_TRIANGLES, 0, object.mesh->get_vertex_count());
  }

  unsigned int error = glGetError();
  if (error) {
    std::cout << "failed to render, code: " << error << "\n";
  }
}
 
void Renderer::render(Scene& scene) {
  scene.prepare_render();
  for(RenderObject* o : scene.objects) {
    draw(*o, scene.camera);
  } 
}
}