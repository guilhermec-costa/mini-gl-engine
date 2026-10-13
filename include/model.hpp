#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "mesh.hpp"
#include "shader.hpp"
#include <memory>
#include <string>

namespace Engine {

class Model {
public:
  explicit Model(const std::string& path, Shader& shader);

private:
  void load_model(const std::string& path);
  void process_node(aiNode* node, const aiScene* scene);
  Mesh process_mesh(aiMesh* mesh, const aiScene* scene);
  std::unique_ptr<Material> process_material(
    aiMaterial* material, 
    const std::string& directory
  );

private:
  Shader& shader;
  std::vector<Mesh> meshes;
  std::vector<std::unique_ptr<Material>> materials;
  std::string directory;
};

}