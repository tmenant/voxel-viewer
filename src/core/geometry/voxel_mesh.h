#pragma once

#include "glm/ext/vector_float3.hpp"
#include <cstdint>
#include <vector>

/*      y
        |
       [4]-------------[7]
      / |             / |
     /  |            /  |
   [5]-------------[6]  |
    |   |           |   |
    |   |           |   |
    |  [0]----------|--[3] -- x
    |  /            |  /
    | /             | /
    [1]-------------[2]
   /
  z
*/

struct VoxelMesh
{
    std::vector<float> vertices;
    std::vector<uint32_t> indices;
    std::vector<uint32_t> lineIndices;

    VoxelMesh(const VoxelMesh &) = delete;
    VoxelMesh &operator=(const VoxelMesh &) = delete;

    VoxelMesh(const std::vector<glm::vec3> &voxels)
    {
        int vIndex = 0;
        int iIndex = 0;
        int lIndex = 0;

        vertices.resize(voxels.size() * 24);
        indices.resize(voxels.size() * 36);
        lineIndices.resize(voxels.size() * 24);

        for (const glm::vec3 &position : voxels)
        {
            int currentVertex = vIndex / 3;

            createVertices(vIndex, position);
            createTrianglesIndices(iIndex, currentVertex);
            createLineIndices(lIndex, currentVertex);
        }
    }

    void createVertices(int &vIndex, const glm::vec3 &position)
    {
        // vertice 0
        vertices[vIndex++] = position.x - 0.5f;
        vertices[vIndex++] = position.y - 0.5f;
        vertices[vIndex++] = position.z - 0.5f;

        // vertice 1
        vertices[vIndex++] = position.x - 0.5f;
        vertices[vIndex++] = position.y - 0.5f;
        vertices[vIndex++] = position.z + 0.5f;

        // vertice 2
        vertices[vIndex++] = position.x + 0.5f;
        vertices[vIndex++] = position.y - 0.5f;
        vertices[vIndex++] = position.z + 0.5f;

        // vertice 3
        vertices[vIndex++] = position.x + 0.5f;
        vertices[vIndex++] = position.y - 0.5f;
        vertices[vIndex++] = position.z - 0.5f;

        // vertice 4
        vertices[vIndex++] = position.x - 0.5f;
        vertices[vIndex++] = position.y + 0.5f;
        vertices[vIndex++] = position.z - 0.5f;

        // vertice 5
        vertices[vIndex++] = position.x - 0.5f;
        vertices[vIndex++] = position.y + 0.5f;
        vertices[vIndex++] = position.z + 0.5f;

        // vertice 6
        vertices[vIndex++] = position.x + 0.5f;
        vertices[vIndex++] = position.y + 0.5f;
        vertices[vIndex++] = position.z + 0.5f;

        // vertice 7
        vertices[vIndex++] = position.x + 0.5f;
        vertices[vIndex++] = position.y + 0.5f;
        vertices[vIndex++] = position.z - 0.5f;
    }

    void createTrianglesIndices(int &iIndex, int currentVertex)
    {
        // bottom
        indices[iIndex++] = currentVertex + 0;
        indices[iIndex++] = currentVertex + 2;
        indices[iIndex++] = currentVertex + 1;

        indices[iIndex++] = currentVertex + 0;
        indices[iIndex++] = currentVertex + 3;
        indices[iIndex++] = currentVertex + 2;

        // top
        indices[iIndex++] = currentVertex + 4;
        indices[iIndex++] = currentVertex + 5;
        indices[iIndex++] = currentVertex + 6;

        indices[iIndex++] = currentVertex + 6;
        indices[iIndex++] = currentVertex + 7;
        indices[iIndex++] = currentVertex + 4;

        // front
        indices[iIndex++] = currentVertex + 1;
        indices[iIndex++] = currentVertex + 2;
        indices[iIndex++] = currentVertex + 6;

        indices[iIndex++] = currentVertex + 6;
        indices[iIndex++] = currentVertex + 5;
        indices[iIndex++] = currentVertex + 1;

        // rear
        indices[iIndex++] = currentVertex + 0;
        indices[iIndex++] = currentVertex + 4;
        indices[iIndex++] = currentVertex + 7;

        indices[iIndex++] = currentVertex + 7;
        indices[iIndex++] = currentVertex + 3;
        indices[iIndex++] = currentVertex + 0;

        // right
        indices[iIndex++] = currentVertex + 3;
        indices[iIndex++] = currentVertex + 7;
        indices[iIndex++] = currentVertex + 6;

        indices[iIndex++] = currentVertex + 6;
        indices[iIndex++] = currentVertex + 2;
        indices[iIndex++] = currentVertex + 3;

        // left
        indices[iIndex++] = currentVertex + 0;
        indices[iIndex++] = currentVertex + 1;
        indices[iIndex++] = currentVertex + 5;

        indices[iIndex++] = currentVertex + 5;
        indices[iIndex++] = currentVertex + 4;
        indices[iIndex++] = currentVertex + 0;
    }

    void createLineIndices(int &lIndex, int currentVertex)
    {
        lineIndices[lIndex++] = currentVertex + 0;
        lineIndices[lIndex++] = currentVertex + 1;

        lineIndices[lIndex++] = currentVertex + 0;
        lineIndices[lIndex++] = currentVertex + 3;

        lineIndices[lIndex++] = currentVertex + 1;
        lineIndices[lIndex++] = currentVertex + 2;

        lineIndices[lIndex++] = currentVertex + 2;
        lineIndices[lIndex++] = currentVertex + 3;

        lineIndices[lIndex++] = currentVertex + 4;
        lineIndices[lIndex++] = currentVertex + 5;

        lineIndices[lIndex++] = currentVertex + 5;
        lineIndices[lIndex++] = currentVertex + 6;

        lineIndices[lIndex++] = currentVertex + 6;
        lineIndices[lIndex++] = currentVertex + 7;

        lineIndices[lIndex++] = currentVertex + 7;
        lineIndices[lIndex++] = currentVertex + 4;

        lineIndices[lIndex++] = currentVertex + 0;
        lineIndices[lIndex++] = currentVertex + 4;

        lineIndices[lIndex++] = currentVertex + 1;
        lineIndices[lIndex++] = currentVertex + 5;

        lineIndices[lIndex++] = currentVertex + 2;
        lineIndices[lIndex++] = currentVertex + 6;

        lineIndices[lIndex++] = currentVertex + 3;
        lineIndices[lIndex++] = currentVertex + 7;
    }
};