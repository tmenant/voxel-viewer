// Renderer.hpp
#pragma once

#include <glad/glad.h>
#include <cstddef>
#include <cstdint>
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/vector_float3.hpp"
#include "shader.h"
#include "camera.h"
#include "world/cavemap.h"
#include "core/geometry/voxel_mesh.h"

class Renderer
{
private:
    Shader m_shader;

    GLuint m_vao = 0;
    GLuint m_vbo = 0;
    GLuint m_ebo = 0;

    GLuint m_vaoEdges;
    GLuint m_eboEdges;

    size_t triangleIndicesCount;
    size_t lineIndicesCount;

public:
    glm::vec3 center;

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
        glClearColor(0.12f, 0.12f, 0.14f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void renderWorld(const Camera &camera)
    {
        m_shader.use();

        m_shader.setVec4("u_Color", glm::vec4(0.8f, 0.8f, 0.8f, 1.0f));
        m_shader.setMat4("u_View", camera.getViewMatrix());
        m_shader.setMat4("u_Projection", camera.getProjectionMatrix());

        glm::mat4 model = glm::mat4(1.0f);
        m_shader.setMat4("u_Model", model);

        glEnable(GL_POLYGON_OFFSET_FILL);
        glPolygonOffset(1.0f, 1.0f);

        glBindVertexArray(m_vao);
        glDrawElements(GL_TRIANGLES, triangleIndicesCount, GL_UNSIGNED_INT, nullptr);

        glDisable(GL_POLYGON_OFFSET_FILL);

        m_shader.setVec4("u_Color", glm::vec4(0.f, 0.f, 0.f, 1.0f));

        glBindVertexArray(m_vaoEdges);
        glDrawElements(GL_LINES, lineIndicesCount, GL_UNSIGNED_INT, nullptr);

        glBindVertexArray(0);
    }

private:
    void setupCubeMesh()
    {
        CaveMap caveMap;
        VoxelMesh mesh(caveMap.voxels);

        center = caveMap.center;

        triangleIndicesCount = mesh.indices.size();
        lineIndicesCount = mesh.lineIndices.size();

        // voxel faces
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

        // wireframe
        glGenVertexArrays(1, &m_vaoEdges);
        glBindVertexArray(m_vaoEdges);
        glGenBuffers(1, &m_eboEdges);
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_eboEdges);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.lineIndices.size() * sizeof(uint32_t), mesh.lineIndices.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);

        glBindVertexArray(0);
    }
};