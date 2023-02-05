#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
{
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;

    SetupMesh();
}

void Mesh::Draw(Shader& shader)
{
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;

    for (unsigned int i = 0; i < textures.size(); i++) {
        std::string number;
        std::string name = textures[i].GetType();
        if (name == "diffuse") {
            number = std::to_string(diffuseNr++);
        } else if (name == "specular") {
            number = std::to_string(specularNr++);
        }

        shader.SetInt(("material." + name + number).c_str(), i);
        textures[i].Bind(i);
    }

    vao.Bind();
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    vao.Unbind();
}

void Mesh::SetupMesh()
{
    vao.Bind();
    vbo = VertexBuffer(&vertices[0], vertices.size() * sizeof(Vertex), GL_STATIC_DRAW);
    vbo.Bind();
    ebo = IndexBuffer(&indices[0], indices.size() * sizeof(unsigned int), GL_STATIC_DRAW);
    ebo.Bind();

    // vertex positions
    vao.LinkAttrib(0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
    // vertex normals
    vao.LinkAttrib(1, 3, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    // vertex texture coordinates
    vao.LinkAttrib(2, 2, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, tex_coords));
}
