#pragma once

#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <assimp/Importer.hpp>
#include <memory>
#include <string>

#include "material.hpp"
#include "mesh.hpp"
#include "shader.hpp"

namespace Engine {

class Model {
 public:
  Model(const std::string& path, Shader& shader);
  std::vector<Mesh>& get_meshes() { return meshes; };

 private:
  void load_model(const std::string& path);
  void process_node(aiNode* node, const aiScene* scene);
  Mesh process_mesh(aiMesh* mesh, const aiScene* scene);

  std::unique_ptr<Material> process_material(aiMaterial* material, const std::string& directory,
                                             const aiScene* scene);

 private:
  Shader& shader;
  std::vector<Mesh> meshes;
  std::vector<std::unique_ptr<Material>> materials;
  std::string directory;
};

}  // namespace Engine