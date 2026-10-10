#include "resource/texture_storage.hpp"

namespace Eng {

TextureStorage& TextureStorage::instance() {
    static TextureStorage storage;
    return storage;
  }

Texture2D& TextureStorage::load(const TextureId id, const std::string &path) {
  Texture2D* stored = get(id);
  if(stored) 
    return *stored;

  auto tex = std::make_unique<Texture2D>(make_texture(path.c_str()));
  auto& ref = *tex;
  textures[id] = std::move(tex);
  return ref;
}

Texture2D* TextureStorage::get(const TextureId id) {
  auto it = textures.find(id);
  if(it == textures.end()) {
    return nullptr;
  }
  return it->second.get();
}

}