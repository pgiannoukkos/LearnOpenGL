#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glm/glm.hpp>
#include <stb_image.h>

#include "Mesh.h"

#include <string>
#include <vector>

class Model
{
public:
    std::vector<Texture2D> textures_loaded;
    std::vector<Mesh> meshes;
    std::string directory;

    Model(const char* path) { LoadModel(path); }

    void Draw(Shader& shader);

private:
    void LoadModel(std::string path);
    void ProcessNode(aiNode* node, const aiScene* scene);
    Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture2D> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string type_name);
    unsigned int TextureFromFile(const char* path, const std::string& directory);
};
