#pragma once

#include <string>
#include <imgui.h>

#include "app_window.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "inputs.h"
#include "rendering/camera.h"
#include "rendering/renderer.h"
#include "utils/logger.h"

class Application : public Loggable<Application>
{
private:
    InputManager m_input;
    AppWindow m_window;
    Camera m_camera;
    Renderer m_renderer;

public:
    Application(const std::string &name, int width, int height) : m_window(name, width, height, m_input)
    {
        m_camera.setPosition(m_renderer.center);
    }

    void run()
    {
        float lastFrame = 0.0f;

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO &io = ImGui::GetIO();
        (void)io;
        ImGui::StyleColorsDark();
        ImGui_ImplGlfw_InitForOpenGL(m_window.getNativeWindow(), true);
        ImGui_ImplOpenGL3_Init("#version 130");

        bool show_demo_window = true;

        while (!m_window.shouldClose())
        {
            float currentFrame = static_cast<float>(glfwGetTime());
            float deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;

            m_window.pollEvents();

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            if (show_demo_window)
            {
                ImGui::ShowDemoWindow(&show_demo_window);
            }

            processInputs(deltaTime);

            ImGui::Render();

            m_renderer.clear();
            m_renderer.renderWorld(m_camera);

            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            m_window.swapBuffers();
        }
    }

private:
    void processInputs(float dt)
    {
        m_camera.processInputs(dt, m_input);

        if (m_input.isMouseButtonPressed(GLFW_MOUSE_BUTTON_RIGHT))
        {
            glfwSetInputMode(m_window.getNativeWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
        else
        {
            glfwSetInputMode(m_window.getNativeWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }

        if (m_input.isKeyPressed(GLFW_KEY_ESCAPE))
        {
            m_window.close();
        }
    }
};