// Renderer.hpp
#pragma once

#include <glad/glad.h>
#include <cstddef>
#include <cstdint>
#include <vector>
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/vector_float3.hpp"
#include "shader.h"
#include "camera.h"
#include "world/voxel_world.h"
#include "core/geometry/voxel.h"

class Renderer
{
private:
    Shader m_shader;

    GLuint m_vao = 0;
    GLuint m_vbo = 0;
    GLuint m_ebo = 0;

    size_t verticesCount;

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
        glDrawElements(GL_TRIANGLES, verticesCount, GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);
    }

private:
    void setupCubeMesh()
    {
        std::vector<glm::vec3> positions = {
            { 0, 0, 0 },
            { 0, 2, 0 },
        };

        VoxelMesh mesh(positions);

        verticesCount = mesh.indices.size();

        glGenVertexArrays(1, &m_vao);

        glGenBuffers(1, &m_vbo);
        glGenBuffers(1, &m_ebo);

        glBindVertexArray(m_vao);

        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferData(GL_ARRAY_BUFFER, mesh.vertices.size() * sizeof(float), mesh.vertices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indices.size() * sizeof(uint32_t), mesh.indices.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);

        glBindVertexArray(0);
    }
};