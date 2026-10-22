// Renderer.hpp
#pragma once

#include <glad/glad.h>
#include "glm/ext/matrix_float4x4.hpp"
#include "shader.h"
#include "camera.h"
#include "world/voxel_world.h"

class Renderer
{
private:
    Shader m_shader;

    GLuint m_vao = 0;
    GLuint m_vbo = 0;
    GLuint m_ebo = 0;

public:
    Renderer() : m_shader("shaders/voxel.vert", "shaders/voxel.frag")
    {
        glEnable(GL_DEPTH_TEST);
        setupCubeMesh();
    }

    ~Renderer()
    {
        glDeleteVertexArrays(1, &m_vao);
        glDeleteBuffers(1, &m_vbo);
        glDeleteBuffers(1, &m_ebo);
    }

    void clear()
    {
        // Couleur de fond (gris foncé) + nettoyage couleur et profondeur
        glClearColor(0.12f, 0.12f, 0.14f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void renderWorld(const VoxelWorld &world, const Camera &camera)
    {
        m_shader.use();

        m_shader.setMat4("u_View", camera.getViewMatrix());
        m_shader.setMat4("u_Projection", camera.getProjectionMatrix());

        glm::mat4 model = glm::mat4(1.0f);
        m_shader.setMat4("u_Model", model);

        glBindVertexArray(m_vao);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);
    }

private:
    void setupCubeMesh()
    {
        // clang-format off
        float vertices[] = {
            // Face avant
            -0.5f, -0.5f,  0.5f,
             0.5f, -0.5f,  0.5f,
             0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f,  0.5f,
            // Face arrière
            -0.5f, -0.5f, -0.5f,
             0.5f, -0.5f, -0.5f,
             0.5f,  0.5f, -0.5f,
            -0.5f,  0.5f, -0.5f,
        };

        unsigned int indices[] = {
            // Avant
            0, 1, 2,  2, 3, 0,
            // Arrière
            5, 4, 7,  7, 6, 5,
            // Gauche
            4, 0, 3,  3, 7, 4,
            // Droite
            1, 5, 6,  6, 2, 1,
            // Haut
            3, 2, 6,  6, 7, 3,
            // Bas
            4, 5, 1,  1, 0, 4
        };
        // clang-format on

        glGenVertexArrays(1, &m_vao);
        glGenBuffers(1, &m_vbo);
        glGenBuffers(1, &m_ebo);

        glBindVertexArray(m_vao);

        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        // Attribut 0 : Position (3 floats)
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);

        glBindVertexArray(0);
    }
};