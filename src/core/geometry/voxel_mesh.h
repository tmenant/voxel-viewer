#pragma once

#include "glm/ext/vector_float3.hpp"
#include <cstddef>
#include <cstdint>
#include <unordered_set>
#include <vector>

/*      y
        |
       [4]-------------[7]
      / |             / |
     /  |            /  |
   [5]-------------[6]  |
    |   |           |   |
    |   |           |   |
    |  [0]----------|--[1] -- x
    |  /            |  /
    | /             | /
    [3]-------------[2]
   /
  z
*/

inline uint64_t packPosition(float x, float y, float z)
{
    uint64_t ux = static_cast<uint16_t>(x);
    uint64_t uy = static_cast<uint16_t>(y);
    uint64_t uz = static_cast<uint16_t>(z);

    return ux | (uz << 16) | (uy << 32);
}

struct VoxelMesh
{
    std::vector<float> vertices;
    std::vector<uint32_t> indices;
    std::vector<uint32_t> lineIndices;

    VoxelMesh(const VoxelMesh &) = delete;
    VoxelMesh &operator=(const VoxelMesh &) = delete;

    VoxelMesh(const std::vector<glm::vec3> &voxels)
    {
        createOptimizedMesh(voxels);
    }

    void createOptimizedMesh(const std::vector<glm::vec3> &voxels)
    {
        int vIndex = 0;
        int iIndex = 0;
        int lIndex = 0;

        vertices.reserve(voxels.size() * 24);
        indices.reserve(voxels.size() * 36);
        lineIndices.reserve(voxels.size() * 24);

        std::unordered_set<uint64_t> voxelsLookup(voxels.size());

        auto isAir = [&](float x, float y, float z) -> bool
        {
            return voxelsLookup.find(packPosition(x, y, z)) == voxelsLookup.end();
        };

        for (const auto &pos : voxels)
        {
            voxelsLookup.insert(packPosition(pos.x, pos.y, pos.z));
        }

        for (const auto &pos : voxels)
        {
            if (isAir(pos.x + 1, pos.y, pos.z)) addRightFace(pos);
            if (isAir(pos.x - 1, pos.y, pos.z)) addLeftFace(pos);
            if (isAir(pos.x, pos.y + 1, pos.z)) addTopFace(pos);
            if (isAir(pos.x, pos.y - 1, pos.z)) addBottomFace(pos);
            if (isAir(pos.x, pos.y, pos.z + 1)) addFrontFace(pos);
            if (isAir(pos.x, pos.y, pos.z - 1)) addBackFace(pos);
        }
    }

    void addQuad(float v0[], float v1[], float v2[], float v3[])
    {
        size_t currentVertex = vertices.size() / 3;

        // vertices
        vertices.push_back(v0[0]);
        vertices.push_back(v0[1]);
        vertices.push_back(v0[2]);

        vertices.push_back(v1[0]);
        vertices.push_back(v1[1]);
        vertices.push_back(v1[2]);

        vertices.push_back(v2[0]);
        vertices.push_back(v2[1]);
        vertices.push_back(v2[2]);

        vertices.push_back(v3[0]);
        vertices.push_back(v3[1]);
        vertices.push_back(v3[2]);

        // triangles indices
        indices.push_back(currentVertex + 0);
        indices.push_back(currentVertex + 1);
        indices.push_back(currentVertex + 2);

        indices.push_back(currentVertex + 2);
        indices.push_back(currentVertex + 3);
        indices.push_back(currentVertex + 0);

        // lines indices
        lineIndices.push_back(currentVertex + 0);
        lineIndices.push_back(currentVertex + 1);

        lineIndices.push_back(currentVertex + 1);
        lineIndices.push_back(currentVertex + 2);

        lineIndices.push_back(currentVertex + 2);
        lineIndices.push_back(currentVertex + 3);

        lineIndices.push_back(currentVertex + 3);
        lineIndices.push_back(currentVertex + 0);
    }

    void addRightFace(const glm::vec3 &pos)
    {
        float v1[] = { pos.x + .5f, pos.y - .5f, pos.z - .5f };
        float v7[] = { pos.x + .5f, pos.y + .5f, pos.z - .5f };
        float v6[] = { pos.x + .5f, pos.y + .5f, pos.z + .5f };
        float v2[] = { pos.x + .5f, pos.y - .5f, pos.z + .5f };

        addQuad(v1, v7, v6, v2);
    }

    void addLeftFace(const glm::vec3 &pos)
    {
        float v0[] = { pos.x - .5f, pos.y - .5f, pos.z - .5f };
        float v3[] = { pos.x - .5f, pos.y - .5f, pos.z + .5f };
        float v5[] = { pos.x - .5f, pos.y + .5f, pos.z + .5f };
        float v4[] = { pos.x - .5f, pos.y + .5f, pos.z - .5f };

        addQuad(v0, v3, v5, v4);
    }

    void addFrontFace(const glm::vec3 &pos)
    {
        float v5[] = { pos.x - .5f, pos.y + .5f, pos.z + .5f };
        float v3[] = { pos.x - .5f, pos.y - .5f, pos.z + .5f };
        float v2[] = { pos.x + .5f, pos.y - .5f, pos.z + .5f };
        float v6[] = { pos.x + .5f, pos.y + .5f, pos.z + .5f };

        addQuad(v5, v3, v2, v6);
    }

    void addBackFace(const glm::vec3 &pos)
    {
        float v0[] = { pos.x - .5f, pos.y - .5f, pos.z - .5f };
        float v4[] = { pos.x - .5f, pos.y + .5f, pos.z - .5f };
        float v7[] = { pos.x + .5f, pos.y + .5f, pos.z - .5f };
        float v1[] = { pos.x + .5f, pos.y - .5f, pos.z - .5f };

        addQuad(v0, v4, v7, v1);
    }

    void addTopFace(const glm::vec3 &pos)
    {
        float v4[] = { pos.x - .5f, pos.y + .5f, pos.z - .5f };
        float v5[] = { pos.x - .5f, pos.y + .5f, pos.z + .5f };
        float v6[] = { pos.x + .5f, pos.y + .5f, pos.z + .5f };
        float v7[] = { pos.x + .5f, pos.y + .5f, pos.z - .5f };

        addQuad(v4, v5, v6, v7);
    }

    void addBottomFace(const glm::vec3 &pos)
    {
        float v0[] = { pos.x - .5f, pos.y - .5f, pos.z - .5f };
        float v1[] = { pos.x + .5f, pos.y - .5f, pos.z - .5f };
        float v2[] = { pos.x + .5f, pos.y - .5f, pos.z + .5f };
        float v3[] = { pos.x - .5f, pos.y - .5f, pos.z + .5f };

        addQuad(v0, v1, v2, v3);
    }
};