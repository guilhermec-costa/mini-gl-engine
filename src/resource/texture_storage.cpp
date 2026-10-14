#include "resource/texture_storage.hpp"

#include <stdexcept>

#include "texture2d.hpp"

namespace Engine {

TextureStorage& TextureStorage::instance() {
  static TextureStorage storage;
  return storage;
}

Texture2D& TextureStorage::load(const std::string& path) {
  Texture2D* stored = get(path);
  if (stored) return *stored;

  auto tex = std::make_unique<Texture2D>(make_texture(path.c_str()));
  auto& ref = *tex;
  textures[path] = std::move(tex);
  return ref;
}

Texture2D& TextureStorage::load(const std::string& key, const aiTexture* embedded) {
  Texture2D* stored = get(key);
  if (stored) return *stored;

  int width, height, channels;
  unsigned char* data = nullptr;

  std::unique_ptr<Texture2D> texture;

  if (embedded->mHeight == 0) {
    texture = std::make_unique<Texture2D>(reinterpret_cast<const unsigned char*>(embedded->pcData),
                                          static_cast<int>(embedded->mWidth));
  } else {
    texture = std::make_unique<Texture2D>(embedded->pcData, embedded->mWidth, embedded->mHeight);
  }

  Texture2D& ref = *texture;
  textures[key] = std::move(texture);
  return ref;
}

Texture2D* TextureStorage::get(const std::string& path) const {
  auto it = textures.find(path);
  if (it == textures.end()) return nullptr;

  return it->second.get();
}

}  // namespace Engine