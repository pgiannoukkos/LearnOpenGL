#pragma once

#include "defines.h"

#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <imgui.h>

#include <GLFW/glfw3.h>

class ImGuiLayer
{
public:
    ImGuiLayer();
    ~ImGuiLayer() = default;

    void OnAttach(GLFWwindow* window);
    void OnDetach();
    void OnImGuiRender(ImTextureID texture, ImVec2 image_size);

    void Begin();
    void End();
};
