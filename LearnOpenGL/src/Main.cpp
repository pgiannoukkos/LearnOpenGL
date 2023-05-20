#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <cmath>
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

static void GLFWErrorCallback(int error, const char* description)
{
    LOG_ERROR("GLFW Error {0}: {1}", error, description);
}

// settings
const float ASPECT_RATIO = 16.0f / 9.0f;
const int SCR_WIDTH = 1280;
const int SCR_HEIGHT = static_cast<int>(SCR_WIDTH / ASPECT_RATIO);

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 5.0f));
float last_x = SCR_WIDTH / 2.0f;
float last_y = SCR_HEIGHT / 2.0f;
bool first_mouse = true;

// timing
float delta_time = 0.0f; // Time between current frame and last frame

// lighting
glm::vec3 light_pos(1.2f, 1.0f, 2.0f);

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

    // initialize ImGui
    // ----------------
    ImGuiLayer* imgui_layer = new ImGuiLayer();
    imgui_layer->OnAttach(window);

    // build and compile shader program
    // --------------------------------
    // Shader lighting_shader("LearnOpenGL/assets/shaders/lighting_vs.glsl",
    // "LearnOpenGL/assets/shaders/lighting_fs.glsl");
    Shader light_cube_shader("assets/shaders/light_cube_vs.glsl", "assets/shaders/light_cube_fs.glsl");
    Shader shader("assets/shaders/model_loading_vs.glsl", "assets/shaders/model_loading_fs.glsl");

    // load models
    // Model our_model("assets/models/obj/backpack/backpack.obj");
    // Model our_model("assets/models/obj/rifle/MA5D_Assault_Rifle_v008.obj");
    // Model our_model("assets/models/obj/workshop/workshop.obj");
    // Model our_model("assets/models/obj/cyborg/cyborg.obj");
    // Model our_model("assets/models/obj/castle/castle.obj");
    Model our_model("assets/models/obj/sponza/sponza.obj");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, 0.0f, 0.0f, 0.5f,  -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, 1.0f, 0.0f,
        0.5f,  0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f, 1.0f, 1.0f, 0.5f,  0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f, 1.0f, 1.0f,
        -0.5f, 0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, 0.0f, 0.0f,

        -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.5f,  -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f, 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
        -0.5f, 0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f, -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,

        -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f,  0.0f,  1.0f, 0.0f, -0.5f, 0.5f,  -0.5f, -1.0f, 0.0f,  0.0f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f,  0.0f,  0.0f, 1.0f, -0.5f, -0.5f, -0.5f, -1.0f, 0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f,  -1.0f, 0.0f,  0.0f,  0.0f, 0.0f, -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f,  0.0f,  1.0f, 0.0f,

        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.5f,  0.5f,  -0.5f, 1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        0.5f,  -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,  0.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        0.5f,  -0.5f, 0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,  0.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,  1.0f, 1.0f,
        0.5f,  -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,  1.0f, 0.0f, 0.5f,  -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,  1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,  0.0f, 0.0f, -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,  0.0f, 1.0f,

        -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.5f,  0.5f,  -0.5f, 0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f, 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f, 0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  0.0f,  0.0f, 1.0f};

    // // positions of all containers
    // glm::vec3 cubePositions[] = {
    //     glm::vec3(0.0f, 0.0f, 0.0f),
    //     glm::vec3(2.0f, 5.0f, -15.0f),
    //     glm::vec3(-1.5f, -2.2f, -2.5f),
    //     glm::vec3(-3.8f, -2.0f, -12.3f),
    //     glm::vec3(2.4f, -0.4f, -3.5f),
    //     glm::vec3(-1.7f, 3.0f, -7.5f),
    //     glm::vec3(1.3f, -2.0f, -2.5f),
    //     glm::vec3(1.5f, 2.0f, -2.5f),
    //     glm::vec3(1.5f, 0.2f, -1.5f),
    //     glm::vec3(-1.3f, 1.0f, -1.5f)
    // };

    // positions of the point lights
    glm::vec3 pointLightPositions[] = {glm::vec3(0.7f, 0.2f, 2.0f), glm::vec3(2.3f, -3.3f, -4.0f),
                                       glm::vec3(-4.0f, 2.0f, -12.0f), glm::vec3(0.0f, 0.0f, -3.0f)};

    // // configure the cube's VAO and VBO
    // VertextArray cube_vao;
    // cube_vao.Bind();

    VertexBuffer vbo(vertices, sizeof(vertices), GL_STATIC_DRAW);
    vbo.Bind();

    // cube_vao.LinkAttrib(0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
    // cube_vao.LinkAttrib(1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    // cube_vao.LinkAttrib(2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    // cube_vao.Unbind();

    // configure the light's VAO
    VertextArray light_vao;
    light_vao.Bind();
    // vbo.Bind();
    light_vao.LinkAttrib(0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
    light_vao.Unbind();
    // vbo.Unbind();

    // // load textures
    // Texture diffuse_map("LearnOpenGL/assets/textures/container2.png", "diffuse");
    // Texture specular_map("LearnOpenGL/assets/textures/container2_specular.png", "specular");

    // shder configuration

    // uncomment this call to draw in wireframe polygons.
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    float last_time = static_cast<float>(glfwGetTime());

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
        delta_time = current_time - last_time;
        last_time = current_time;

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

        // lighting_shader.Use();
        shader.SetVec3("view_pos", camera.m_Position);

        // directional light
        shader.SetVec3("dir_light.direction", -0.2f, -1.0f, -0.3f);
        shader.SetVec3("dir_light.ambient", 0.05f, 0.05f, 0.05f);
        shader.SetVec3("dir_light.diffuse", 0.4f, 0.4f, 0.4f);
        shader.SetVec3("dir_light.specular", 0.5f, 0.5f, 0.5f);

        // point light 1
        shader.SetVec3("point_lights[0].position", pointLightPositions[0]);
        shader.SetVec3("point_lights[0].ambient", 0.05f, 0.05f, 0.05f);
        shader.SetVec3("point_lights[0].diffuse", 0.8f, 0.8f, 0.8f);
        shader.SetVec3("point_lights[0].specular", 1.0f, 1.0f, 1.0f);
        shader.SetFloat("point_lights[0].constant", 1.0f);
        shader.SetFloat("point_lights[0].linear", 0.09f);
        shader.SetFloat("point_lights[0].quadratic", 0.032f);
        // point light 2
        shader.SetVec3("point_lights[1].position", pointLightPositions[1]);
        shader.SetVec3("point_lights[1].ambient", 0.05f, 0.05f, 0.05f);
        shader.SetVec3("point_lights[1].diffuse", 0.8f, 0.8f, 0.8f);
        shader.SetVec3("point_lights[1].specular", 1.0f, 1.0f, 1.0f);
        shader.SetFloat("point_lights[1].constant", 1.0f);
        shader.SetFloat("point_lights[1].linear", 0.09f);
        shader.SetFloat("point_lights[1].quadratic", 0.032f);
        // point light 3
        shader.SetVec3("point_lights[2].position", pointLightPositions[2]);
        shader.SetVec3("point_lights[2].ambient", 0.05f, 0.05f, 0.05f);
        shader.SetVec3("point_lights[2].diffuse", 0.8f, 0.8f, 0.8f);
        shader.SetVec3("point_lights[2].specular", 1.0f, 1.0f, 1.0f);
        shader.SetFloat("point_lights[2].constant", 1.0f);
        shader.SetFloat("point_lights[2].linear", 0.09f);
        shader.SetFloat("point_lights[2].quadratic", 0.032f);
        // point light 4
        shader.SetVec3("point_lights[3].position", pointLightPositions[3]);
        shader.SetVec3("point_lights[3].ambient", 0.05f, 0.05f, 0.05f);
        shader.SetVec3("point_lights[3].diffuse", 0.8f, 0.8f, 0.8f);
        shader.SetVec3("point_lights[3].specular", 1.0f, 1.0f, 1.0f);
        shader.SetFloat("point_lights[3].constant", 1.0f);
        shader.SetFloat("point_lights[3].linear", 0.09f);
        shader.SetFloat("point_lights[3].quadratic", 0.032f);

        // spotLight
        shader.SetVec3("spot_light.position", camera.m_Position);
        shader.SetVec3("spot_light.direction", camera.m_Front);
        shader.SetVec3("spot_light.ambient", 0.0f, 0.0f, 0.0f);
        shader.SetVec3("spot_light.diffuse", 1.0f, 1.0f, 1.0f);
        shader.SetVec3("spot_light.specular", 1.0f, 1.0f, 1.0f);
        shader.SetFloat("spot_light.constant", 1.0f);
        shader.SetFloat("spot_light.linear", 0.09f);
        shader.SetFloat("spot_light.quadratic", 0.032f);
        shader.SetFloat("spot_light.cut_off", glm::cos(glm::radians(12.5f)));
        shader.SetFloat("spot_light.outer_cut_off", glm::cos(glm::radians(15.0f)));

        // material properties
        // lighting_shader.SetInt("material.diffuse", 0);
        // lighting_shader.SetInt("material.specular", 1);
        shader.SetFloat("material.shininess", 64.0f);

        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.m_Zoom), ASPECT_RATIO, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        shader.SetMat4("projection", projection);
        shader.SetMat4("view", view);

        // world transformation
        glm::mat4 model = glm::mat4(1.0f);
        model =
            glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the cornen of the screen
        model =
            glm::scale(model, glm::vec3(0.005f, 0.005f, 0.005f)); // it's a bit too big for our scene, so scale it down
        // model = glm::rotate(model, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0));
        shader.SetMat4("model", model);
        our_model.Draw(shader);

        // // bind diffuse map
        // diffuse_map.Bind(0);
        // specular_map.Bind(1);

        // // render the cube
        // cube_vao.Bind();
        // for (unsigned int i = 0; i < 10; i++) {
        //     glm::mat4 model = glm::mat4(1.0f);
        //     model = glm::translate(model, cubePositions[i]);
        //     float angle = 20.0f * i;
        //     model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
        //     lighting_shader.SetMat4("model", model);
        //     glDrawArrays(GL_TRIANGLES, 0, 36);
        // }

        // also draw the lamp object
        light_cube_shader.Use();
        light_cube_shader.SetMat4("projection", projection);
        light_cube_shader.SetMat4("view", view);
        light_vao.Bind();
        for (unsigned int i = 0; i < 4; i++) {
            model = glm::mat4(1.0f);
            model = glm::translate(model, pointLightPositions[i]);
            model = glm::scale(model, glm::vec3(0.2f));
            light_cube_shader.SetMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

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
    light_vao.Destroy();
    // cube_vao.Destroy();
    vbo.Destroy();
    // lighting_shader.Destroy();
    light_cube_shader.Destroy();
    shader.Destroy();

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
