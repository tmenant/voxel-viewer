#pragma once

#include <string>

#include "app_window.h"
#include "inputs.h"
#include "rendering/camera.h"
#include "rendering/renderer.h"
#include "world/voxel_world.h"

class Application
{
private:
    InputManager m_input;
    AppWindow m_window;
    Camera m_camera;
    VoxelWorld m_world;
    Renderer m_renderer;

public:
    Application(const std::string &name, int width, int height) : m_window(name, width, height, m_input)
    {
    }

    void run()
    {
        float lastFrame = 0.0f;

        while (!m_window.shouldClose())
        {
            float currentFrame = static_cast<float>(glfwGetTime());
            float deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;

            processInputs(deltaTime);

            m_renderer.clear();
            m_renderer.renderWorld(m_world, m_camera);
            m_window.swapBuffers();
        }
    }

private:
    void processInputs(float dt)
    {
        m_window.pollEvents();

        m_camera.processInputs(dt, m_input);

        if (m_input.isKeyPressed(GLFW_KEY_ESCAPE))
        {
            m_window.close();
        }
    }
};