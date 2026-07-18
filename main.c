
#include "raylib.h"
#include "stdio.h"
#include "stdlib.h"

int main(void)
{
    InitWindow(400, 400, "Hello World");

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);


        EndDrawing();
    }

    CloseWindow();

    return 0;
}
