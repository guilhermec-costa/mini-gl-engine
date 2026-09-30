#include <glad/glad.h>
#include <iostream>
#include "renderer.hpp"

namespace Eng {

void Renderer::draw(const Mesh& mesh) {
  auto material = mesh.get_material();
  if(material)
    material->bind();

  glBindVertexArray(mesh.get_VAO());
  glDrawArrays(GL_TRIANGLES, 0, mesh.get_vertex_count());

  unsigned int error = glGetError();
  if (error) {
    std::cout << "failed to render, code: " << error << "\n";
  }
}
  
}