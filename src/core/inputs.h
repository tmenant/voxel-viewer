#pragma once

#include <GLFW/glfw3.h>

// Mapping des touches spécifiques au clavier AZERTY vers les codes physiques GLFW (QWERTY)
#define AZERTY_KEY_A GLFW_KEY_Q
#define AZERTY_KEY_Z GLFW_KEY_W
#define AZERTY_KEY_Q GLFW_KEY_A
#define AZERTY_KEY_W GLFW_KEY_Z
#define AZERTY_KEY_M GLFW_KEY_SEMICOLON // Le 'M' AZERTY se trouve à la place du point-virgule QWERTY

// Si vous voulez tout unifier pour la propreté de votre code :
#define AZERTY_KEY_B GLFW_KEY_B
#define AZERTY_KEY_C GLFW_KEY_C
#define AZERTY_KEY_D GLFW_KEY_D
#define AZERTY_KEY_E GLFW_KEY_E
#define AZERTY_KEY_F GLFW_KEY_F
#define AZERTY_KEY_G GLFW_KEY_G
#define AZERTY_KEY_H GLFW_KEY_H
#define AZERTY_KEY_I GLFW_KEY_I
#define AZERTY_KEY_J GLFW_KEY_J
#define AZERTY_KEY_K GLFW_KEY_K
#define AZERTY_KEY_L GLFW_KEY_L
#define AZERTY_KEY_N GLFW_KEY_N
#define AZERTY_KEY_O GLFW_KEY_O
#define AZERTY_KEY_P GLFW_KEY_P
#define AZERTY_KEY_R GLFW_KEY_R
#define AZERTY_KEY_S GLFW_KEY_S
#define AZERTY_KEY_T GLFW_KEY_T
#define AZERTY_KEY_U GLFW_KEY_U
#define AZERTY_KEY_V GLFW_KEY_V
#define AZERTY_KEY_X GLFW_KEY_X
#define AZERTY_KEY_Y GLFW_KEY_Y

class InputManager
{
private:
    bool m_keys[GLFW_KEY_LAST] = { false };
    bool m_mouseButtons[GLFW_MOUSE_BUTTON_LAST + 1] = { false };

    double m_mouseX = 0.0;
    double m_mouseY = 0.0;

public:
    InputManager() = default;

    void onKey(int key, int scancode, int action)
    {
        if (key >= 0 && key < 512)
        {
            m_keys[key] = (action != GLFW_RELEASE);
        }
    }

    void onMouseButton(int button, int action, int mods)
    {
        if (button >= 0 && button <= GLFW_MOUSE_BUTTON_LAST)
        {
            m_mouseButtons[button] = (action != GLFW_RELEASE);
        }
    }

    bool isMouseButtonPressed(int button) const
    {
        if (button < 0 || button > GLFW_MOUSE_BUTTON_LAST) return false;
        return m_mouseButtons[button];
    }

    void onMouseMove(double x, double y)
    {
        m_mouseX = x;
        m_mouseY = y;
    }

    bool isKeyPressed(int keycode) const
    {
        if (keycode < 0 || keycode >= 512) return false;

        return m_keys[keycode];
    }

    void getMousePosition(double &x, double &y) const
    {
        x = m_mouseX;
        y = m_mouseY;
    }
};