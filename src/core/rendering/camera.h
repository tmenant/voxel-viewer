#pragma once

#include "GLFW/glfw3.h"
#include "core/inputs.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/geometric.hpp"
#include "glm/trigonometric.hpp"

class Camera
{
private:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;

    float yaw;
    float pitch;

    double lastMouseX = 0.0;
    double lastMouseY = 0.0;
    float mouseSensitivity = 0.1f;
    bool firstClick = true;

    float fov;
    float aspectRatio;
    float nearPlane;
    float farPlane;

    void updateCameraVectors()
    {
        glm::vec3 newFront;
        newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        newFront.y = sin(glm::radians(pitch));
        newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        front = glm::normalize(newFront);

        right = glm::normalize(glm::cross(front, worldUp));
        up = glm::normalize(glm::cross(right, front));
    }

public:
    Camera() :
            position(0.f, 0.f, 0.f),
            worldUp(0.f, 1.f, 0.f),
            yaw(-90.f),
            pitch(0.f),
            fov(45.f),
            nearPlane(0.1f),
            farPlane(100.f),
            aspectRatio(1920.f / 1080.f)
    {
        updateCameraVectors();
    }

    glm::mat4 getViewMatrix() const
    {
        return glm::lookAt(position, position + front, up);
    }

    glm::mat4 getProjectionMatrix() const
    {
        return glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
    }

    void processInputs(float deltaTime, const InputManager &inputs)
    {
        float velocity = 2.5f * deltaTime;

        if (inputs.isKeyPressed(AZERTY_KEY_Z)) position += front * velocity;
        if (inputs.isKeyPressed(AZERTY_KEY_Q)) position -= right * velocity;
        if (inputs.isKeyPressed(AZERTY_KEY_S)) position -= front * velocity;
        if (inputs.isKeyPressed(AZERTY_KEY_D)) position += right * velocity;

        if (inputs.isKeyPressed(GLFW_KEY_SPACE)) position += up * velocity;
        if (inputs.isKeyPressed(GLFW_KEY_C)) position -= up * velocity;

        rotateCamera(deltaTime, inputs);
    }

    void rotateCamera(float deltaTime, const InputManager &inputs)
    {
        if (inputs.isMouseButtonPressed(GLFW_MOUSE_BUTTON_RIGHT))
        {
            double mouseX, mouseY;
            inputs.getMousePosition(mouseX, mouseY);

            // Si c'est le tout premier clic, on initialise les anciennes coordonnées
            // pour éviter que la caméra ne "saute" d'un coup vers la souris.
            if (firstClick)
            {
                lastMouseX = mouseX;
                lastMouseY = mouseY;
                firstClick = false;
            }

            // Calcul du décalage (offset) de la souris
            float xoffset = static_cast<float>(mouseX - lastMouseX);
            float yoffset = static_cast<float>(lastMouseY - mouseY); // Inversé car les Y vont de bas en haut sur l'écran

            // On sauvegarde la position actuelle pour la prochaine frame
            lastMouseX = mouseX;
            lastMouseY = mouseY;

            // On applique la sensibilité
            xoffset *= mouseSensitivity;
            yoffset *= mouseSensitivity;

            // On ajoute aux angles de la caméra
            yaw += xoffset;
            pitch += yoffset;

            // On bloque le pitch (tangage) à 89 degrés pour éviter le "Gimbal Lock" (la caméra qui se retourne)
            if (pitch > 89.0f) pitch = 89.0f;
            if (pitch < -89.0f) pitch = -89.0f;

            // On met à jour les vecteurs de direction de la caméra
            updateCameraVectors();
        }
        else
        {
            firstClick = true;
        }
    }

    void setAspectRatio(float aspect)
    {
        aspectRatio = aspect;
    }
};