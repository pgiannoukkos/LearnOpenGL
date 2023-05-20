#include "Model.h"

#include "Log.h"

Model::Model(const char* path)
{
    LoadModel(path);
}

void Model::Draw(Shader& shader)
{
    for (auto& mesh : meshes) {
        mesh.Draw(shader);
    }
}

void Model::LoadModel(std::string path)
{
    LOG_INFO("Assimp: Loading Model: {0}", path.c_str());
    Assimp::Importer importer;
    u32 flags = aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals;
    const aiScene* scene = importer.ReadFile(path, flags);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        LOG_ERROR("Assimp: {0}", importer.GetErrorString());
        return;
    }

    // retrieve the directory path of the filepath
    directory = path.substr(0, path.find_last_of('/'));

    // process ASSIMP's root node recursively
    ProcessNode(scene->mRootNode, scene);
}

void Model::ProcessNode(aiNode* node, const aiScene* scene)
{
    // process all the node's mashes (if any)
    for (u32 i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(ProcessMesh(mesh, scene));
    }

    // then do the same for each of its children
    for (u32 i = 0; i < node->mNumChildren; i++) {
        ProcessNode(node->mChildren[i], scene);
    }
}

Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector<Vertex> vertices;
    std::vector<u32> indices;
    std::vector<Texture2D> textures;

    for (u32 i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex;

        // process vertex positions, normals and texture coordinates
        // positions
        glm::vec3 vector;
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.position = vector;

        // normals
        if (mesh->HasNormals()) {
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.normal = vector;
        }

        // texture coordinates
        if (mesh->mTextureCoords[0]) {
            glm::vec2 vec;
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.tex_coords = vec;
        }
        else {
            vertex.tex_coords = glm::vec2(0.0f, 0.0f);
        }

        vertices.push_back(vertex);
    }

    // process indices
    for (u32 i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (u32 j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }

    // process material
    if (mesh->mMaterialIndex >= 0) {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

        std::vector<Texture2D> diffuse_maps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuse_maps.begin(), diffuse_maps.end());

        std::vector<Texture2D> specular_maps =
            LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specular_maps.begin(), specular_maps.end());
    }

    return Mesh(vertices, indices, textures);
}

std::vector<Texture2D> Model::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string type_name)
{
    std::vector<Texture2D> textures;

    for (u32 i = 0; i < mat->GetTextureCount(type); i++) {
        aiString str;
        mat->GetTexture(type, i, &str);

        // check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
        bool skip = false;
        for (u32 j = 0; j < textures_loaded.size(); j++) {
            if (std::strcmp(textures_loaded[j].GetPath().data(), str.C_Str()) == 0) {
                textures.push_back(textures_loaded[j]);
                skip = true;
                break;
            }
        }
        if (!skip) {
            Texture2D texture;
            texture = TextureFromFile(str.C_Str(), this->directory);
            texture.SetType(type_name);
            texture.SetPath(str.C_Str());
            textures.push_back(texture);
            textures_loaded.push_back(texture);
        }
    }
    return textures;
}

Texture2D Model::TextureFromFile(const char* path, const std::string& directory)
{
    std::string filename = std::string(path);
    filename = directory + '/' + filename;
    LOG_TRACE("Texture: {0}", filename);

    Texture2D texture;

    stbi_set_flip_vertically_on_load(false);
    i32 width, height, nrComponents;
    u8* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data) {
        u32 format;
        if (nrComponents == 1) {
            format = GL_RED;
            texture.SetInternalFormat(format);
            texture.SetImageFormat(format);
        }
        else if (nrComponents == 3) {
            format = GL_RGB;
            texture.SetInternalFormat(format);
            texture.SetImageFormat(format);
        }
        else if (nrComponents == 4) {
            format = GL_RGBA;
            texture.SetInternalFormat(format);
            texture.SetImageFormat(format);
        }

        texture.Generate(width, height, data, true);

        stbi_image_free(data);
    }
    else {
        LOG_ERROR("Texture: Failed to load {0}", path);
        stbi_image_free(data);
    }

    return texture;
}
