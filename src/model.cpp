#include "model.hpp"
#include "assimp/material.h"
#include "material.hpp"
#include <iostream>
#include <memory>
#include <vector>

namespace Engine {

Model::Model(const std::string &path, Shader &s) : shader(s) {
  load_model(path);
};

void Model::load_model(const std::string &path) {
  Assimp::Importer importer;
  const aiScene *scene = importer.ReadFile(path.c_str(), aiProcess_Triangulate |
                                                             aiProcess_FlipUVs);

  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
      !scene->mRootNode) {

    std::cout << importer.GetErrorString() << std::endl;
    return;
  }

  directory = path.substr(0, path.find_last_of('/'));
  materials.reserve(scene->mNumMaterials);

  for(size_t m = 0; m<scene->mNumMaterials; ++m) {
    materials.push_back(
      process_material(scene->mMaterials[m], directory)
    );
  }
  process_node(scene->mRootNode, scene);
};

void Model::process_node(aiNode *node, const aiScene *scene) {
  for (size_t i = 0; i < node->mNumMeshes; i++) {
    aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
    meshes.push_back(process_mesh(mesh, scene));
  }

  for (size_t i = 0; i < node->mNumChildren; i++) {
    process_node(node->mChildren[i], scene);
  }
};

Mesh Model::process_mesh(aiMesh *mesh, const aiScene *scene) {
  std::vector<float> raw_data;
  std::vector<unsigned int> indices;
  std::vector<float> uvs;

  raw_data.reserve(mesh->mNumVertices * 8);

  for (size_t i = 0; i < mesh->mNumVertices; ++i) {
    const auto &pos = mesh->mVertices[i];
    raw_data.push_back(pos.x);
    raw_data.push_back(pos.y);
    raw_data.push_back(pos.z);

    if (mesh->mTextureCoords[0]) {
      const auto &tex_coord = mesh->mTextureCoords[0][i];
      raw_data.push_back(tex_coord.x);
      raw_data.push_back(tex_coord.y);
    } else {
      raw_data.push_back(0.0f);
      raw_data.push_back(0.0f);
    }

    if (mesh->HasNormals()) {
      const auto &normal = mesh->mNormals[i];
      raw_data.push_back(normal.x);
      raw_data.push_back(normal.y);
      raw_data.push_back(normal.z);
    } else {
      raw_data.push_back(0.0f);
      raw_data.push_back(0.0f);
      raw_data.push_back(0.0f);
    }
  }

  for (size_t f = 0; f < mesh->mNumFaces; ++f) {
    const aiFace &face = mesh->mFaces[f];
    for (size_t i = 0; i < face.mNumIndices; ++i) {
      indices.push_back(face.mIndices[i]);
    }
  }

  Mesh result_mesh(raw_data, indices,
                   std::vector{
                       Engine::VertexAttribute{0, 3, 0},
                       Engine::VertexAttribute{1, 2, sizeof(float) * 3},
                       Engine::VertexAttribute{2, 3, sizeof(float) * 5},
                   },
                   raw_data.size() / 8, sizeof(float) * 8);

  if (mesh->mMaterialIndex < materials.size()) {
    result_mesh.material = materials[mesh->mMaterialIndex].get();
  }

  return result_mesh;
}

std::unique_ptr<Material> Model::process_material(aiMaterial* ai_material, const std::string& directory) {
  auto mat = std::make_unique<Material>(shader);

  if(ai_material->GetTextureCount())
}

} // namespace Engine