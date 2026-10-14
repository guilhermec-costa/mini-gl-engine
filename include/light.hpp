#pragma once

#include "glm/ext/vector_float3.hpp"

namespace Engine {

class DirectionalLight {
 public:
  DirectionalLight() = default;

 public:
  glm::vec3 direction;
  glm::vec3 ambient;
  glm::vec3 diffuse;
  glm::vec3 specular;
};

class PointLight {
 public:
  PointLight() = default;

 public:
  glm::vec3 position;
  glm::vec3 ambient;
  glm::vec3 diffuse;
  glm::vec3 specular;

  float constant;
  float linear;
  float quadratic;
  float radius;
};

class Spotlight {
 public:
  Spotlight() = default;

 public:
  glm::vec3 position;
  glm::vec3 direction;
  glm::vec3 ambient;
  glm::vec3 diffuse;
  glm::vec3 specular;

  float constant;
  float linear;
  float quadratic;
  float radius;
  float cut_off;
  float outer_cut_off;
};

}  // namespace Engine