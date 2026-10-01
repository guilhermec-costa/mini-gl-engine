#include <glad/glad.h>
#include <iostream>
#include "renderer.hpp"

namespace Eng {

void Renderer::draw(const RenderObject& object) {
  Material* material = object.mesh.get_material();
  if(material) {
    material->shader.patch_uniform("model", object.transform.matrix());
    material->bind();
  }

  glBindVertexArray(object.mesh.get_VAO());
  if(object.mesh.get_index_count() > 0) {
    glDrawElements(GL_TRIANGLES, object.mesh.get_index_count(), GL_UNSIGNED_INT, nullptr);
  } else {
    glDrawArrays(GL_TRIANGLES, 0, object.mesh.get_vertex_count());
  }

  unsigned int error = glGetError();
  if (error) {
    std::cout << "failed to render, code: " << error << "\n";
  }
}
  
}