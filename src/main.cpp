#include "raylib.h"

const float moveSpeed = 10.f;
const Color backgroundColor = { 64, 64, 64, 255 };
const Color meshColor = { 128, 128, 128, 255 };
const Color vertexColor = { 64, 64, 64, 255 };

void UpdateCamera(Camera *camera)
{
    float dt = GetFrameTime();

    Vector3 movement = { 0.0f, 0.0f, 0.0f };
    Vector3 rotation = { 0.0f, 0.0f, 0.0f };
    Vector2 mouseDelta = GetMouseDelta();

    // 1. Bascule (Toggle) : Au moment EXACT où on appuie sur Entrée
    if (IsKeyPressed(KEY_ENTER))
    {
        if (IsCursorHidden())
        {
            EnableCursor(); // Si elle était cachée, on la libère
        }
        else
        {
            DisableCursor(); // Si elle était libre, on la capture
        }
    }

    // 2. On tourne la caméra UNIQUEMENT si le curseur est actuellement capturé
    if (IsCursorHidden())
    {
        rotation.x = mouseDelta.x * 0.1f;
        rotation.y = mouseDelta.y * 0.1f;
    }

    // --- LE RESTE NE CHANGE PAS ---
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

int main()
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);

    InitWindow(1920, 1080, "Mon super Voxel");
    MaximizeWindow();

    Camera3D camera = { 0 };
    camera.position = (Vector3){ 4.0f, 4.0f, 4.0f };
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    SetTargetFPS(60);

    DisableCursor();

    while (!WindowShouldClose())
    {
        UpdateCamera(&camera);

        BeginDrawing();
        ClearBackground(backgroundColor);

        BeginMode3D(camera);
        DrawCube((Vector3){ 0.0f, 0.0f, 0.0f }, 2.0f, 2.0f, 2.0f, meshColor);
        DrawCubeWires((Vector3){ 0.0f, 0.0f, 0.0f }, 2.0f, 2.0f, 2.0f, vertexColor);
        EndMode3D();

        EndDrawing();
    }

    CloseWindow();
    return 0;
}