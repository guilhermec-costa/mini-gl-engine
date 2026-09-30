#include <glad/glad.h>
#include "primitives.hpp"
#include "albedo.hpp"
#include "material.hpp"
#include "mesh.hpp"
#include "shader.hpp"

void draw_quad(unsigned int width, unsigned int height, glm::vec3 origin) {
  auto shader = Eng::Shader::create(shaderpath("vertex.glsl").c_str(), shaderpath("frag.glsl").c_str());
  Albedo albedo(albedopath("wood.jpg").c_str(), GL_RGB, GL_RGB);
  Eng::Material material(*shader, albedo);

  // Eng::Mesh m1(
  //   std::vector{
  //     0.0f, 0.5f, 0.0f, 0.5f, 1.0f,
  //     0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
  //     -0.5f, -0.5f, 0.0f, 1.0f, 0.0f
  //   },
  //   std::vector{
  //     VertexAttribute(0, 3, 0),
  //     VertexAttribute(1, 2, sizeof(float) * 3)
  //   },
  //   3,
  //   sizeof(float) * 5,
  //   &material
  // ); 
}