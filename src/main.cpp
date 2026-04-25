#include "raylib.h"

int main()
{
    // 1. Initialisation de la fenêtre
    InitWindow(800, 600, "Cube avec Raylib");

    // 2. Configuration d'une caméra 3D
    Camera3D camera = { 0 };
    camera.position = (Vector3){ 4.0f, 4.0f, 4.0f }; // On se recule un peu
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };   // On regarde le centre
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    SetTargetFPS(60);

    // 3. Boucle principale
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        BeginMode3D(camera);
        // On dessine le cube directement en une ligne !
        DrawCube((Vector3){ 0.0f, 0.0f, 0.0f }, 2.0f, 2.0f, 2.0f, RED);
        // Optionnel : on dessine les arêtes pour mieux voir la 3D
        DrawCubeWires((Vector3){ 0.0f, 0.0f, 0.0f }, 2.0f, 2.0f, 2.0f, MAROON);
        EndMode3D();

        EndDrawing();
    }

    CloseWindow();
    return 0;
}