#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include <iostream>
#include <cmath>

#include "Shader.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Camera.h"
#include "Texture.h"
#include "Model.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int heigth);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void process_input(GLFWwindow* window);
void InitImGui(GLFWwindow* window);

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


    // initialize ImGui
    // ----------------
    InitImGui(window);

    // build and compile shader program
    // --------------------------------
    // Shader lighting_shader("LearnOpenGL/assets/shaders/lighting_vs.glsl", "LearnOpenGL/assets/shaders/lighting_fs.glsl");
    Shader light_cube_shader("LearnOpenGL/assets/shaders/light_cube_vs.glsl", "LearnOpenGL/assets/shaders/light_cube_fs.glsl");
    Shader shader("LearnOpenGL/assets/shaders/model_loading_vs.glsl", "LearnOpenGL/assets/shaders/model_loading_fs.glsl");

    // load models
    Model our_model("LearnOpenGL/assets/models/backpack/backpack.obj");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
    };

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
    glm::vec3 pointLightPositions[] = {
        glm::vec3(0.7f, 0.2f, 2.0f),
        glm::vec3(2.3f, -3.3f, -4.0f),
        glm::vec3(-4.0f, 2.0f, -12.0f),
        glm::vec3(0.0f, 0.0f, -3.0f)
    };

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

    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

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
        glm::mat4 projection = glm::perspective(glm::radians(camera.m_Zoom), (float)SCR_WIDTH/(float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        shader.SetMat4("projection", projection);
        shader.SetMat4("view", view);

        // world transformation
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the cornen of the screen
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f)); // it's a bit too big for our scene, so scale it down
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

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
        {
            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

            ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
            ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
            ImGui::Checkbox("Another Window", &show_another_window);

            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

            if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }

        // 3. Show another simple window.
        if (show_another_window)
        {
            ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
            ImGui::Text("Hello from another window!");
            if (ImGui::Button("Close Me"))
                show_another_window = false;
            ImGui::End();
        }

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        // glViewport(0, 0, display_w, display_h);
        // glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        // glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Update and Render additional Platform Windows
        // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
        //  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
        if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    // optional: de-allocate all resources once they've outlived their purpose
    // -----------------------------------------------------------------------
    // light_vao.Destroy();
    // cube_vao.Destroy();
    // vbo.Destroy();
    // lighting_shader.Destroy();
    // light_cube_shader.Destroy();

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
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
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

void InitImGui(GLFWwindow* window)
{
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
    //io.ConfigViewportsNoAutoMerge = true;
    //io.ConfigViewportsNoTaskBarIcon = true;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330 core");
}
