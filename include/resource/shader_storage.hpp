#pragma once
#include <unordered_map>

#include "shader.hpp"

enum class ShaderId { Base, ObjectLight };

namespace Engine {

class ShaderStorage {
 public:
  static ShaderStorage& instance();
  Shader& load(const ShaderId id, const char* vertex, const char* fragment);
  Shader* get(const ShaderId id) const;

 private:
  ShaderStorage() = default;
  ShaderStorage(ShaderStorage& other) = delete;
  ShaderStorage& operator=(ShaderStorage& other) = delete;

 private:
  std::unordered_map<ShaderId, std::unique_ptr<Shader>> shaders;
};
}  // namespace Engine