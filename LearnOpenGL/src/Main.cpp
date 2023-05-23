#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include "Camera.h"
#include "ImGui/ImGuiLayer.h"
#include "IndexBuffer.h"
#include "Log.h"
#include "Model.h"
#include "Shader.h"
#include "Texture2D.h"
#include "VertexArray.h"
#include "VertexBuffer.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int heigth);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void process_input(GLFWwindow* window);
void InitImGui(GLFWwindow* window);
void PrintGLFWInfo(GLFWwindow* window);
void PrintGLInfo();
Texture2D TextureFromFile(const char* path);

static void GLFWErrorCallback(int error, const char* description)
{
    LOG_ERROR("GLFW Error {0}: {1}", error, description);
}

// settings
const float ASPECT_RATIO = 16.0f / 9.0f;
const int SCR_WIDTH = 1280;
const int SCR_HEIGHT = static_cast<int>(SCR_WIDTH / ASPECT_RATIO);

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float last_x = SCR_WIDTH / 2.0f;
float last_y = SCR_HEIGHT / 2.0f;
bool first_mouse = true;

// timing
float delta_time = 0.0f; // Time between current frame and last frame
float last_frame = 0.0f;

int main()
{
    // Initialize Logging
    Log::Init();

    // glfw: initialize and configure
    // ------------------------------
    if (!glfwInit()) {
        LOG_FATAL("Failed to initialize GLFW.");
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif // __APPLE__

    // glfw window creation
    LOG_INFO("Creating GLFW window {0}x{1}", SCR_WIDTH, SCR_HEIGHT);
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (!window) {
        LOG_FATAL("Failed to create GLFW window");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        LOG_FATAL("Failed to initialize GLAD");
        glfwTerminate();
        return -1;
    }

    // Print info
    // ---------------------------------
    PrintGLFWInfo(window);
    PrintGLInfo();

    // configure global OpenGL state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);     // Always pass the depth test (same effect as glDisable(GL_DEPTH_TEST))

    // initialize ImGui
    // ----------------
    ImGuiLayer* imgui_layer = new ImGuiLayer();
    imgui_layer->OnAttach(window);

    // build and compile shader program
    // --------------------------------
    Shader shader("assets/shaders/depth_testing_vs.glsl", "assets/shaders/depth_testing_fs.glsl");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float cubeVertices[] = {
        // positions          // texture Coords
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    float planeVertices[] = {
        // positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
         5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
        -5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
        -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,

         5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
        -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
         5.0f, -0.5f, -5.0f,  2.0f, 2.0f
    };

    VertextArray cube_vao;
    cube_vao.Bind();

    VertexBuffer cube_vbo(&cubeVertices, sizeof(cubeVertices), GL_STATIC_DRAW);
    cube_vbo.Bind();
    cube_vao.LinkAttrib(0, 3, GL_FLOAT, 5 * sizeof(f32), (void*)0);
    cube_vao.LinkAttrib(1, 2, GL_FLOAT, 5 * sizeof(f32), (void*)(3 * sizeof(f32)));
    cube_vao.Unbind();
    cube_vbo.Unbind();

    VertextArray plane_vao;
    plane_vao.Bind();

    VertexBuffer plane_vbo(&planeVertices, sizeof(planeVertices), GL_STATIC_DRAW);
    plane_vbo.Bind();
    plane_vao.LinkAttrib(0, 3, GL_FLOAT, 5 * sizeof(f32), (void*)0);
    plane_vao.LinkAttrib(1, 2, GL_FLOAT, 5 * sizeof(f32), (void*)(3 * sizeof(f32)));
    plane_vao.Unbind();
    plane_vbo.Unbind();

    // Load Textures
    // -------------
    Texture2D cube_texture;
    cube_texture = TextureFromFile("assets/textures/marble.jpg");

    Texture2D plane_texture;
    plane_texture = TextureFromFile("assets/textures/metal.png");

    // Shader Configuration
    // --------------------
    shader.Use();
    shader.SetInt("texture1", 0);

    // Framebuffer and Scene Texture setup
    u32 tex_width = 1280;
    u32 tex_height = 720;

    u32 framebuffer;
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    Texture2D scene;
    scene.SetInternalFormat(GL_RGBA);
    scene.SetImageFormat(GL_RGBA);
    scene.Generate(tex_width, tex_height, nullptr);

    // Attach the texture to the framebuffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, scene.GetTexID(), 0);

    u32 rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, tex_width, tex_height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo);

    // Check framebuffer completeness
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        LOG_ERROR("Framebuffer incomplete");
        return 1;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window)) {
        // per-frame time logic
        float current_time = static_cast<float>(glfwGetTime());
        delta_time = current_time - last_frame;
        last_frame = current_time;

        // input
        process_input(window);

        // render
        // ------
        // bind to framebuffer and draw scene as we normally would to color texture
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        glEnable(GL_DEPTH_TEST);                 // enable depth testing
        glViewport(0, 0, tex_width, tex_height); // set glViewport to texture's dimensions

        // clear the framebuffer's contents
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.Use();
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(camera.m_Zoom), ASPECT_RATIO, 0.1f, 100.0f);
        shader.SetMat4("view", view);
        shader.SetMat4("projection", projection);

        // cubes
        cube_vao.Bind();
        cube_texture.Bind(0);
        model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
        shader.SetMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
        shader.SetMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        cube_vao.Unbind();
        cube_texture.UnBind();

        // floor
        plane_vao.Bind();
        plane_texture.Bind(0);
        model = glm::mat4(1.0f);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        plane_vao.Unbind();
        plane_texture.UnBind();

        // bind back to the default framebuffer
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDisable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT);

        // Start the Dear ImGui frame
        imgui_layer->Begin();

        imgui_layer->OnImGuiRender(reinterpret_cast<ImTextureID>(scene.GetTexID()), ImVec2(tex_width, tex_height));

        // Rendering
        imgui_layer->End();

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    cube_vao.Destroy();
    cube_vbo.Destroy();
    plane_vao.Destroy();
    plane_vbo.Destroy();
    shader.Destroy();

    cube_texture.Destroy();
    plane_texture.Destroy();

    glDeleteRenderbuffers(1, &rbo);
    glDeleteFramebuffers(1, &framebuffer);

    imgui_layer->OnDetach();

    // glfw: terminate, clearing all previously allocated GLFW resources.
    glfwTerminate();
    return 0;
}

void process_input(GLFWwindow* window)
{
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
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        camera.m_MovementSpeed = 10.0f;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
        camera.m_MovementSpeed = 5.0f;

    // const float speed = static_cast<float>(5.0f * delta_time);
    // if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    //     light_pos.x += speed;
    // if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    //     light_pos.x -= speed;
    // if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    //     light_pos.y += speed;
    // if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    //     light_pos.y -= speed;
    // if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS)
    //     light_pos.z += speed;
    // if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS)
    //     light_pos.z -= speed;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int heigth)
{
    glViewport(0, 0, width, heigth);
}

void mouse_callback(GLFWwindow* window, double xpos_in, double ypos_in)
{
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        float xpos = static_cast<float>(xpos_in);
        float ypos = static_cast<float>(ypos_in);

        if (first_mouse) {
            last_x = xpos;
            last_y = ypos;
            first_mouse = false;
        }

        float xoffset = xpos - last_x;
        float yoffset = last_y - ypos; // reversed since y-coordinates go from bottom to top

        last_x = xpos;
        last_y = ypos;

        camera.ProcessMouseMovement(xoffset, yoffset);
    }
    else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        first_mouse = true;
    }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

void PrintGLFWInfo(GLFWwindow* window)
{
    int p = glfwGetWindowAttrib(window, GLFW_OPENGL_PROFILE);
    std::string version = glfwGetVersionString();
    std::string opengl_profile = "";

    if (p == GLFW_OPENGL_COMPAT_PROFILE) {
        opengl_profile = "OpenGL Compatibility Profile";
    }
    else if (p == GLFW_OPENGL_CORE_PROFILE) {
        opengl_profile = "OpenGL Core Profile";
    }

    LOG_INFO("GLFW: {0}", version.c_str());
    LOG_INFO("GLFW: {0} {1}", opengl_profile.c_str(), p);
}

void PrintGLInfo()
{
    LOG_INFO("OpenGL: GL version {0}", (const char*)glGetString(GL_VERSION));
    LOG_INFO("OpenGL: GLSL version: {0}", (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION));
    LOG_INFO("OpenGL: Vendor: {0}", (const char*)glGetString(GL_VENDOR));
    LOG_INFO("OpenGL: Renderer: {0}", (const char*)glGetString(GL_RENDERER));
}

Texture2D TextureFromFile(const char* path)
{
    LOG_TRACE("Texture: {0}", path);

    Texture2D texture;

    stbi_set_flip_vertically_on_load(true);
    i32 width, height, nrComponents;
    u8* data = stbi_load(path, &width, &height, &nrComponents, 0);
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
