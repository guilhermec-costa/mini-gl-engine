#include "resource/texture_storage.hpp"
#include "texture2d.hpp"

namespace Engine {

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

Texture2D& TextureStorage::load(const std::string& path) {
  Texture2D* stored = get(path);
  if(stored) 
    return *stored;

  auto tex = std::make_unique<Texture2D>(make_texture(path.c_str()));
  auto& ref = *tex;
  imported_textures[path] = std::move(tex);
  return ref;
}

Texture2D* TextureStorage::get(const TextureId id) const {
  auto it = textures.find(id);
  if(it == textures.end()) {
    return nullptr;
  }
  return it->second.get();
}

Texture2D* TextureStorage::get(const std::string& path) const {
  auto it = imported_textures.find(path);
  if(it == imported_textures.end()) 
    return nullptr;

  return it->second.get();
}

}