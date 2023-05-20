#include "Mesh.h"
#include "Log.h"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<u32> indices, std::vector<Texture2D> textures)
{
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;

    SetupMesh();
}

void Mesh::Draw(Shader& shader)
{
    u32 diffuseNr = 1;
    u32 specularNr = 1;
    u32 normalNr = 1;

    for (u32 i = 0; i < textures.size(); i++)
    {
        textures[i].Bind(i);
        std::string number;
        std::string name = textures[i].GetType();
        if (name == "texture_diffuse")
        {
            number = std::to_string(diffuseNr++);
        }
        else if (name == "texture_specular")
        {
            number = std::to_string(specularNr++);
        }
        else if (name == "texture_normal")
        {
            number = std::to_string(normalNr++);
        }


        shader.SetInt(("material." + name + number).c_str(), i);
        // glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }

    vao.Bind();
    glDrawElements(GL_TRIANGLES, static_cast<int>(indices.size()), GL_UNSIGNED_INT, 0);
    vao.Unbind();

    glActiveTexture(GL_TEXTURE0);
}

void Mesh::SetupMesh()
{
    vao.Bind();
    vbo = VertexBuffer(vertices.data(), vertices.size() * sizeof(Vertex), GL_STATIC_DRAW);
    vbo.Bind();
    ebo = IndexBuffer(indices.data(), indices.size() * sizeof(u32), GL_STATIC_DRAW);
    ebo.Bind();

    // vertex positions
    vao.LinkAttrib(0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
    // vertex normals
    vao.LinkAttrib(1, 3, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    // vertex texture coordinates
    vao.LinkAttrib(2, 2, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, tex_coords));
    // vertex tangent
    vao.LinkAttrib(3, 3, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, tangents));
    // vertex bitangent
    vao.LinkAttrib(4, 3, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, bitangents));
}
