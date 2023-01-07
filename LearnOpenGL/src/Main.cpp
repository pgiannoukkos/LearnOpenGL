#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <cmath>

#include "Shader.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int heigth);
void process_input(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

int main() {
    // glfw: initialize and configure
    // ------------------------------
    if (!glfwInit()) {
        std::cout << "Failed to initialize GLFW." << std::endl;
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif // __APPLE__

    // glfw window creation
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (!window) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        return -1;
    }

    // build and compile shader program
    // --------------------------------
    Shader shader("../LearnOpenGL/assets/shaders/vertex.glsl", "../LearnOpenGL/assets/shaders/fragment.glsl");

    int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    std::cout << "Maximum number of vertex attributes supported: " << nrAttributes << std::endl;

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        // positions        //colors          // texture coords
         0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,     // top right
         0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,     // bottom right
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,     // bottom left
        -0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f      // top left
    };

    unsigned int indices[] = {
        0, 1, 3,    // first triangle
        1, 2, 3     // second triangle
    };

    float tex_coords[] = {
        0.0f, 0.0f, // lower-left corner
        1.0f, 0.0f, // lower-right corner
        0.0f, 1.0f, // upper-left corner
        1.0f, 1.0f  // upper-right corner
    };

    VertextArray vao;
    vao.Bind();

    VertexBuffer vbo(vertices, sizeof(vertices), GL_STATIC_DRAW);
    vbo.Bind();

    IndexBuffer ebo(indices, sizeof(indices), GL_STATIC_DRAW);
    ebo.Bind();

    vao.LinkAttrib(0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
    vao.LinkAttrib(1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    vao.LinkAttrib(2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));

    // this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    vbo.Unbind();

    // do NOT unbind the EBO while a VAO is active as the bound EBO is stored in the VAO; keep the EBO bound
    // ebo.Unbind();

    // we can unbind the VAO afterwards so othe VAO call won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    vao.Unbind();

    // uncomment this call to draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // load and create a texture
    // -------------------------
    unsigned int texture1, texture2;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);

    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // set the texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded textures on the y-axis
    unsigned char* data = stbi_load("../LearnOpenGL/assets/textures/container.jpg", &width, &height, &nrChannels, 0);
    if (!data) {
        std::cout << "Failed to load texture" << std::endl;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);

    // load and create a texture
    // -------------------------
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);

    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // set the texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // load image, create texture and generate mipmaps
    // int width, height, nrChannels;
    data = stbi_load("../LearnOpenGL/assets/textures/awesomeface.png", &width, &height, &nrChannels, 0);
    if (!data) {
        std::cout << "Failed to load texture" << std::endl;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);

    shader.Use();
    shader.SetInt("texture1", 0);
    shader.SetInt("texture2", 1);

    double last_time = glfwGetTime();
    int nb_frames = 0;

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window)) {
        double current_time = glfwGetTime();
        nb_frames++;

        if (current_time - last_time >= 1.0) {
            double ms_per_frame = 1000.0 / double(nb_frames);
            printf("%.2f ms/frame - %d fps\n", ms_per_frame, nb_frames);
            nb_frames = 0;
            last_time += 1.0;
        }

        // input
        process_input(window);

        // render
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // bind texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        glm::mat4 transform = glm::mat4(1.0f);
        transform = glm::translate(transform, glm::vec3(0.5, -0.5, 0.0));
        transform = glm::scale(transform, glm::vec3(0.5, 0.5, 0.5));
        transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(0.0, 0.0, 1.0));
        shader.SetMat4("transform", transform);

        // activate the shader
        shader.Use();

        // render the quad
        vao.Bind();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        vao.Unbind();

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    // optional: de-allocate all resources once they've outlived their purpose
    // -----------------------------------------------------------------------
    vao.Destroy();
    vbo.Destroy();
    ebo.Destroy();
    shader.Destroy();

    // glfw: terminate, clearing all previously allocated GLFW resources.
    glfwTerminate();
    return 0;
}

void process_input(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int heigth) {
    glViewport(0, 0, width, heigth);
}
