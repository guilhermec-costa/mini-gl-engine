#include "resource/shader_storage.hpp"

using namespace Engine;

namespace Engine {

ShaderStorage& ShaderStorage::instance() {
  static ShaderStorage _in;
  return _in;
}

Shader& ShaderStorage::load(const ShaderId id, const char* vertex, const char* fragment) {
  auto shader = Shader::create(shaderpath(vertex).c_str(), shaderpath(fragment).c_str());

  if (!shader) {
    throw std::runtime_error("failed to compile shader: " + std::string(shader.error()));
  }

  auto s = std::make_unique<Shader>(std::move(*shader));
  Shader& ref = *s;
  shaders[id] = std::move(s);
  return ref;
};

Shader* ShaderStorage::get(const ShaderId id) const {
  auto it = shaders.find(id);
  if (it == shaders.end()) return nullptr;

  return it->second.get();
}
}  // namespace Engine