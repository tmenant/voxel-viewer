#include <raylib.h>
#include <cstdint>
#include <vector>

#include "cavemap.h"

const float moveSpeed = 10.f;
const Color backgroundColor = { 64, 64, 64, 255 };
const Color meshColor = { 128, 128, 128, 128 };
const Color vertexColor = { 64, 64, 64, 255 };

void UpdateCamera(Camera *camera)
{
    float dt = GetFrameTime();

    Vector3 movement = { 0.0f, 0.0f, 0.0f };
    Vector3 rotation = { 0.0f, 0.0f, 0.0f };
    Vector2 mouseDelta = GetMouseDelta();

    if (IsKeyPressed(KEY_ENTER))
    {
        if (IsCursorHidden())
        {
            EnableCursor();
        }
        else
        {
            DisableCursor();
        }
    }

    if (IsCursorHidden())
    {
        rotation.x = mouseDelta.x * 0.1f;
        rotation.y = mouseDelta.y * 0.1f;
    }

    float currentSpeed = moveSpeed;

    if (IsKeyDown(KEY_LEFT_SHIFT))
    {
        currentSpeed *= 2.5f;
    }

    if (IsKeyDown(KEY_W)) movement.x += currentSpeed * dt;
    if (IsKeyDown(KEY_S)) movement.x -= currentSpeed * dt;

    if (IsKeyDown(KEY_A)) movement.y -= currentSpeed * dt;
    if (IsKeyDown(KEY_D)) movement.y += currentSpeed * dt;

    if (IsKeyDown(KEY_SPACE)) movement.z += currentSpeed * dt;
    if (IsKeyDown(KEY_C)) movement.z -= currentSpeed * dt;

    UpdateCameraPro(camera, movement, rotation, 0.0f);
}

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

struct VoxelMesh
{
    Mesh mesh = { 0 };
    Model voxelFace;

    VoxelMesh(const VoxelMesh &) = delete;
    VoxelMesh &operator=(const VoxelMesh &) = delete;

    VoxelMesh(const std::vector<Vector3> &voxels)
    {
        mesh.triangleCount = voxels.size() * 16;
        mesh.vertexCount = voxels.size() * 8;

        mesh.vertices = (float *)MemAlloc(mesh.vertexCount * 3 * sizeof(float));
        mesh.indices = (unsigned short *)MemAlloc(mesh.triangleCount * 3 * sizeof(unsigned short));

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

        int vIndex = 0;
        int iIndex = 0;

        for (const Vector3 &position : voxels)
        {
            int currentVertex = vIndex / 3;

            // vertice 0
            mesh.vertices[vIndex++] = position.x - 0.5f;
            mesh.vertices[vIndex++] = position.y - 0.5f;
            mesh.vertices[vIndex++] = position.z - 0.5f;

            // vertice 1
            mesh.vertices[vIndex++] = position.x - 0.5f;
            mesh.vertices[vIndex++] = position.y - 0.5f;
            mesh.vertices[vIndex++] = position.z + 0.5f;

            // vertice 2
            mesh.vertices[vIndex++] = position.x + 0.5f;
            mesh.vertices[vIndex++] = position.y - 0.5f;
            mesh.vertices[vIndex++] = position.z + 0.5f;

            // vertice 3
            mesh.vertices[vIndex++] = position.x + 0.5f;
            mesh.vertices[vIndex++] = position.y - 0.5f;
            mesh.vertices[vIndex++] = position.z - 0.5f;

            // vertice 4
            mesh.vertices[vIndex++] = position.x - 0.5f;
            mesh.vertices[vIndex++] = position.y + 0.5f;
            mesh.vertices[vIndex++] = position.z - 0.5f;

            // vertice 5
            mesh.vertices[vIndex++] = position.x - 0.5f;
            mesh.vertices[vIndex++] = position.y + 0.5f;
            mesh.vertices[vIndex++] = position.z + 0.5f;

            // vertice 6
            mesh.vertices[vIndex++] = position.x + 0.5f;
            mesh.vertices[vIndex++] = position.y + 0.5f;
            mesh.vertices[vIndex++] = position.z + 0.5f;

            // vertice 7
            mesh.vertices[vIndex++] = position.x + 0.5f;
            mesh.vertices[vIndex++] = position.y + 0.5f;
            mesh.vertices[vIndex++] = position.z - 0.5f;

            // bottom
            mesh.indices[iIndex++] = currentVertex + 0;
            mesh.indices[iIndex++] = currentVertex + 2;
            mesh.indices[iIndex++] = currentVertex + 1;

            mesh.indices[iIndex++] = currentVertex + 0;
            mesh.indices[iIndex++] = currentVertex + 3;
            mesh.indices[iIndex++] = currentVertex + 2;

            // front
            mesh.indices[iIndex++] = currentVertex + 1;
            mesh.indices[iIndex++] = currentVertex + 2;
            mesh.indices[iIndex++] = currentVertex + 6;

            mesh.indices[iIndex++] = currentVertex + 1;
            mesh.indices[iIndex++] = currentVertex + 6;
            mesh.indices[iIndex++] = currentVertex + 5;

            // right
            mesh.indices[iIndex++] = currentVertex + 6;
            mesh.indices[iIndex++] = currentVertex + 2;
            mesh.indices[iIndex++] = currentVertex + 3;

            mesh.indices[iIndex++] = currentVertex + 6;
            mesh.indices[iIndex++] = currentVertex + 3;
            mesh.indices[iIndex++] = currentVertex + 7;

            // top
            mesh.indices[iIndex++] = currentVertex + 4;
            mesh.indices[iIndex++] = currentVertex + 5;
            mesh.indices[iIndex++] = currentVertex + 6;

            mesh.indices[iIndex++] = currentVertex + 4;
            mesh.indices[iIndex++] = currentVertex + 6;
            mesh.indices[iIndex++] = currentVertex + 7;

            // back
            mesh.indices[iIndex++] = currentVertex + 4;
            mesh.indices[iIndex++] = currentVertex + 7;
            mesh.indices[iIndex++] = currentVertex + 3;

            mesh.indices[iIndex++] = currentVertex + 4;
            mesh.indices[iIndex++] = currentVertex + 3;
            mesh.indices[iIndex++] = currentVertex + 0;

            // left
            mesh.indices[iIndex++] = currentVertex + 4;
            mesh.indices[iIndex++] = currentVertex + 0;
            mesh.indices[iIndex++] = currentVertex + 1;

            mesh.indices[iIndex++] = currentVertex + 4;
            mesh.indices[iIndex++] = currentVertex + 1;
            mesh.indices[iIndex++] = currentVertex + 5;
        }

        UploadMesh(&mesh, false);

        voxelFace = LoadModelFromMesh(mesh);
        voxelFace.materials[0].maps[MATERIAL_MAP_DIFFUSE].color = meshColor;
    }

    ~VoxelMesh()
    {
        UnloadModel(voxelFace);
    }

    void draw()
    {
        DrawModel(voxelFace, (Vector3){ 0.0f, 0.0f, 0.0f }, 1.0f, RAYWHITE);
        DrawModelWires(voxelFace, (Vector3){ 0.0f, 0.0f, 0.0f }, 1.0f, backgroundColor);
    }
};

int main()
{

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);

    InitWindow(1920, 1080, "Raylib - Voxel Viewer");
    MaximizeWindow();

    Camera3D camera = { 0 };
    camera.position = (Vector3){ 4.0f, 4.0f, 4.0f };
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    SetTargetFPS(120);

    DisableCursor();


    CaveMap cavemap;
    VoxelMesh voxelMesh(cavemap.voxels);

    while (!WindowShouldClose())
    {
        UpdateCamera(&camera);

        BeginDrawing();
        ClearBackground(backgroundColor);

        BeginMode3D(camera);

        voxelMesh.draw();

        EndMode3D();

        EndDrawing();
    }

    CloseWindow();
    return 0;
}