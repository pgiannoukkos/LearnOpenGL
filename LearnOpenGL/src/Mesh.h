#pragma once

#include "defines.h"

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <vector>

#include "IndexBuffer.h"
#include "Shader.h"
#include "Texture2D.h"
#include "VertexArray.h"
#include "VertexBuffer.h"

struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 tex_coords;
};

class Mesh
{
public:
    std::vector<Vertex> vertices;
    std::vector<u32> indices;
    std::vector<Texture2D> textures;

    Mesh(std::vector<Vertex> vertices, std::vector<u32> indices, std::vector<Texture2D> textures);

    void Draw(Shader& shader);

private:
    // render data
    VertextArray vao;
    VertexBuffer vbo;
    IndexBuffer ebo;

    void SetupMesh();
};
