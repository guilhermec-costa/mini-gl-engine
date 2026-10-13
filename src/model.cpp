#include "model.hpp"
#include "assimp/material.h"
#include "assimp/types.h"
#include "macros.hpp"
#include "material.hpp"
#include "texture2d.hpp"
#include <iostream>
#include <memory>
#include <vector>

namespace Engine {

Model::Model(const std::string &path, Shader &s) : shader(s) {
  load_model(path);
};

void Model::load_model(const std::string &path) {
  Assimp::Importer importer;
  const aiScene *scene = importer.ReadFile(
      path.c_str(),
      aiProcess_Triangulate | aiProcess_GenSmoothNormals);

  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
      !scene->mRootNode) {

    std::cout << importer.GetErrorString() << std::endl;
    return;
  }

  directory = path.substr(0, path.find_last_of('/'));
  materials.reserve(scene->mNumMaterials);

  for (size_t m = 0; m < scene->mNumMaterials; ++m) {
    materials.push_back(process_material(scene->mMaterials[m], directory));
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

std::unique_ptr<Material>
Model::process_material(aiMaterial *ai_material, const std::string &directory) {
  auto material = std::make_unique<Material>(shader);
  aiColor3D ambient(1.0f, 1.0f, 1.0f);
  aiColor3D diffuse(1.0f, 1.0f, 1.0f);
  aiColor3D specular(1.0f, 1.0f, 1.0f);
  float shininess = 32.0f;

  if(ai_material->Get(AI_MATKEY_COLOR_AMBIENT, ambient) == AI_SUCCESS) {
    material->set_ambient({
      ambient.r,
      ambient.g,
      ambient.b
    });
  }

  if (ai_material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse) == AI_SUCCESS) {
    material->set_diffuse({
      diffuse.r,
      diffuse.g,
      diffuse.b
    });
  }

  if (ai_material->Get(AI_MATKEY_COLOR_SPECULAR, specular) == AI_SUCCESS) {
    material->set_specular({
      specular.r,
      specular.g,
      specular.b
    });
  }

  if(ai_material->Get(AI_MATKEY_SHININESS, shininess) == AI_SUCCESS) {
    material->set_shininess(shininess);
  }

  if (ai_material->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
    aiString tex_path;

    if (ai_material->GetTexture(aiTextureType_DIFFUSE, 0, &tex_path) ==
        AI_SUCCESS) {
      std::string full_path = directory + "/" + tex_path.C_Str();
      Texture2D &tex = TEXTURES.load(full_path);

      material->set_diffuse_map(&tex);
    }
  }

  if (ai_material->GetTextureCount(aiTextureType_SPECULAR) > 0) {
    aiString tex_path;

    if (ai_material->GetTexture(aiTextureType_SPECULAR, 0, &tex_path) ==
        AI_SUCCESS) {
      std::string full_path = directory + "/" + tex_path.C_Str();
      Texture2D &tex = TEXTURES.load(full_path);
      material->set_specular_map(&tex);
    }
  }

  return material;
}

} // namespace Engine