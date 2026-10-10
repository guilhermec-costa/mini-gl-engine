#pragma once

#include "texture2d.hpp"
#include <memory>
#include <unordered_map>

enum class TextureId {
  StoneWall,
  MedievalBoxDiffuse,
  MedievalBoxSpecular,
  WoodFace,
  Matrix
};

namespace Eng {

class TextureStorage {
public:
  static TextureStorage& instance();
  Texture2D &load(const TextureId, const std::string &path);
  Texture2D* get(const TextureId);

private:
  TextureStorage() = default;
  TextureStorage(const TextureStorage&) = delete;
  TextureStorage& operator=(const TextureStorage&) = delete;

private:
  std::unordered_map<TextureId, std::unique_ptr<Texture2D>> textures;
};

} // namespace Eng