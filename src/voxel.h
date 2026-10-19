#pragma once

#include <cstdint>
#include <vector>

enum VoxelFace
{
    Top,
    Bottom,
    Front,
    Back,
    Right,
    Left,
};

struct VoxelMask
{
private:
    uint8_t mask = 0x3F;

public:
    inline bool get(VoxelFace face) const
    {
        return mask & (1 << face);
    }

    inline void set(VoxelFace face, bool value)
    {
        if (value)
        {
            mask |= (1 << face);
        }
        else
        {
            mask &= ~(1 << face);
        }
    }

    inline void clear()
    {
        mask = 0;
    }

    inline void setAll()
    {
        mask = 0x3F;
    }
};

struct vec3
{
    int x;
    int y;
    int z;
};

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

    VoxelMesh(const VoxelMesh &) = delete;
    VoxelMesh &operator=(const VoxelMesh &) = delete;

    VoxelMesh(const std::vector<vec3> &voxels)
    {
        int vIndex = 0;
        int iIndex = 0;

        vertices.resize(voxels.size() * 8);

        for (const vec3 &position : voxels)
        {
            int currentVertex = vIndex / 3;

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
    }
};