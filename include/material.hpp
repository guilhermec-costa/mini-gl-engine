#pragma once

#include "albedo.hpp"
#include "shader.hpp"

namespace Eng {

class Material {
public:
  Material() = delete;
  Material(Shader& shader): shader(shader) {};
  Material(Shader& shader, Albedo& albedo): shader(shader), albedo(&albedo) {};
  void bind() const;
  void set_albedo(Albedo& albedo);

public:
  Shader& shader;
  Albedo* albedo = nullptr;
};
}