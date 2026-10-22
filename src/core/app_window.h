#pragma once

#include <glad/glad.h>
#include "GLFW/glfw3.h"
#include "core/inputs.h"

#include <stdexcept>

class AppWindow
{
private:
    GLFWwindow *m_window = nullptr;

    int m_width = 0;
    int m_height = 0;

    std::string m_title = "OpenGL Window";

public:
    AppWindow(const std::string &title, int width, int height, InputManager &inputManager) :
            m_width(width),
            m_height(height)
    {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        m_window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
        if (m_window == NULL)
        {
            glfwTerminate();
            throw std::runtime_error("Failed to create GLFW window");
        }

        glfwMakeContextCurrent(m_window);
        glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            throw std::runtime_error("Failed to initialize GLAD");
        }

        // attach an inputManager reference to the current window
        glfwSetWindowUserPointer(m_window, &inputManager);

        // register glfw key & mouse callbacks
        glfwSetKeyCallback(m_window, [](GLFWwindow *win, int key, int scancode, int action, int mods)
        {
            auto *input = static_cast<InputManager *>(glfwGetWindowUserPointer(win));
            if (input) input->onKey(key, scancode, action);
        });

        glfwSetCursorPosCallback(m_window, [](GLFWwindow *win, double xpos, double ypos)
        {
            auto *input = static_cast<InputManager *>(glfwGetWindowUserPointer(win));
            if (input) input->onMouseMove(xpos, ypos);
        });
    }

    ~AppWindow()
    {
        if (m_window)
        {
            glfwDestroyWindow(m_window);
        }

        glfwTerminate();
    }

    GLFWwindow *getNativeWindow() const { return m_window; }

    int getWidth() const { return m_width; }
    int getHeight() const { return m_height; }

    // Gestion du cycle de vie
    bool shouldClose() const { return glfwWindowShouldClose(m_window); }
    void swapBuffers() const { glfwSwapBuffers(m_window); }
    void pollEvents() const { glfwPollEvents(); }

    void processInput(GLFWwindow *window)
    {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window, true);
        }
    }

    void close()
    {
        glfwTerminate();
    }

private:
    static void framebuffer_size_callback(GLFWwindow *window, int width, int height)
    {
        glViewport(0, 0, width, height);
    }
};