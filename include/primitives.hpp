#pragma once

#include "glm/ext/vector_float3.hpp"
#include "material.hpp"
#include "mesh.hpp"

Eng::Mesh make_quad(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, glm::vec3 v4, Eng::Material* material);
Eng::Mesh make_triangle(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, Eng::Material* material);
