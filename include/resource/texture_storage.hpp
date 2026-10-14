#pragma once

#include <memory>
#include <unordered_map>

#include "assimp/texture.h"
#include "texture2d.hpp"

namespace Engine {

class TextureStorage {
 public:
  static TextureStorage& instance();
  Texture2D& load(const std::string& path);
  Texture2D& load(const std::string& key, const aiTexture* embedded);
  Texture2D* get(const std::string& path) const;

 private:
  TextureStorage() = default;
  TextureStorage(const TextureStorage&) = delete;
  TextureStorage& operator=(const TextureStorage&) = delete;

 private:
  std::unordered_map<std::string, std::unique_ptr<Texture2D>> textures;
};

}  // namespace Engine