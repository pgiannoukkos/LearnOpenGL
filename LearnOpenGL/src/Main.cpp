#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <cmath>

#include "Shader.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Camera.h"
#include "Texture.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int heigth);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void process_input(GLFWwindow* window);

// settings
const int SCR_WIDTH = 800;
const int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 5.0f));
float last_x = SCR_WIDTH / 2.0f;
float last_y = SCR_HEIGHT / 2.0f;
bool first_mouse = true;

// timing
float delta_time = 0.0f;    // Time between current frame and last frame

// lighting
glm::vec3 light_pos(1.2f, 1.0f, 2.0f);

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
    glfwSwapInterval(0);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        return -1;
    }

    // configure global OpenGL state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile shader program
    // --------------------------------
    Shader lighting_shader("LearnOpenGL/assets/shaders/lighting_vs.glsl", "LearnOpenGL/assets/shaders/lighting_fs.glsl");
    Shader light_cube_shader("LearnOpenGL/assets/shaders/light_cube_vs.glsl", "LearnOpenGL/assets/shaders/light_cube_fs.glsl");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,

        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f};

    // configure the cube's VAO and VBO
    VertextArray cube_vao;
    cube_vao.Bind();

    VertexBuffer vbo(vertices, sizeof(vertices), GL_STATIC_DRAW);
    vbo.Bind();

    cube_vao.LinkAttrib(0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
    cube_vao.LinkAttrib(1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    cube_vao.LinkAttrib(2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    cube_vao.Unbind();

    // configure the light's VAO
    VertextArray light_vao;
    light_vao.Bind();
    vbo.Bind();
    light_vao.LinkAttrib(0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
    light_vao.Unbind();
    vbo.Unbind();

    // load textures
    Texture diffuse_map("LearnOpenGL/assets/textures/container2.png");
    Texture specular_map("LearnOpenGL/assets/textures/container2_specular.png");

    // uncomment this call to draw in wireframe polygons.
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    float last_time = static_cast<float>(glfwGetTime());

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window)) {
        // per-frame time logic
        float current_time = static_cast<float>(glfwGetTime());
        delta_time = current_time - last_time;
        last_time = current_time;

        // input
        process_input(window);

        // render
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        lighting_shader.Use();
        lighting_shader.SetVec3("light.position", light_pos);
        lighting_shader.SetVec3("view_pos", camera.m_Position);

        // light properties
        lighting_shader.SetVec3("light.ambient", glm::vec3(0.2f));
        lighting_shader.SetVec3("light.diffuse", glm::vec3(0.5f));     // darken diffuse light a bit
        lighting_shader.SetVec3("light.specular", glm::vec3(1.0f));

        // material properties
        lighting_shader.SetInt("material.diffuse", 0);
        lighting_shader.SetInt("material.specular", 1);
        lighting_shader.SetFloat("material.shininess", 64.0f);

        glm::mat4 projection = glm::perspective(glm::radians(camera.m_Zoom), (float)SCR_WIDTH/(float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        lighting_shader.SetMat4("projection", projection);
        lighting_shader.SetMat4("view", view);

        // world transformation
        glm::mat4 model = glm::mat4(1.0f);
        lighting_shader.SetMat4("model", model);

        // bind diffuse map
        diffuse_map.Bind(0);
        specular_map.Bind(1);

        // render the cube
        cube_vao.Bind();
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // also draw the lamp object
        light_cube_shader.Use();
        light_cube_shader.SetMat4("projection", projection);
        light_cube_shader.SetMat4("view", view);
        model = glm::mat4(1.0f);
        model = glm::translate(model, light_pos);
        model = glm::scale(model, glm::vec3(0.2f));
        light_cube_shader.SetMat4("model", model);

        light_vao.Bind();
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    // optional: de-allocate all resources once they've outlived their purpose
    // -----------------------------------------------------------------------
    light_vao.Destroy();
    cube_vao.Destroy();
    vbo.Destroy();
    lighting_shader.Destroy();
    light_cube_shader.Destroy();

    // glfw: terminate, clearing all previously allocated GLFW resources.
    glfwTerminate();
    return 0;
}

void process_input(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, delta_time);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, delta_time);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, delta_time);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, delta_time);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.ProcessKeyboard(UPWARDS, delta_time);
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        camera.ProcessKeyboard(DOWNWARDS, delta_time);

    const float speed = static_cast<float>(5.0f * delta_time);
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        light_pos.x += speed;
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        light_pos.x -= speed;
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        light_pos.y += speed;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        light_pos.y -= speed;
    if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS)
        light_pos.z += speed;
    if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS)
        light_pos.z -= speed;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int heigth) {
    glViewport(0, 0, width, heigth);
}

void mouse_callback(GLFWwindow* window, double xpos_in, double ypos_in) {
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        float xpos = static_cast<float>(xpos_in);
        float ypos = static_cast<float>(ypos_in);

        if (first_mouse)
        {
            last_x = xpos;
            last_y = ypos;
            first_mouse = false;
        }

        float xoffset = xpos - last_x;
        float yoffset = last_y - ypos; // reversed since y-coordinates go from bottom to top

        last_x = xpos;
        last_y = ypos;

        camera.ProcessMouseMovement(xoffset, yoffset);
    } else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        first_mouse = true;
    }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
