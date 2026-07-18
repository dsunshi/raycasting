
#include "raylib.h"
#include "stdio.h"
#include "stdlib.h"

// Image
int image_width  = 256;
int image_height = 256;

int main(void)
{
        InitWindow(image_width, image_height, "Hello World");

        while (!WindowShouldClose()) {
                BeginDrawing();
                /* ClearBackground(RAYWHITE); */

                for (int j = 0; j < image_height; j++) {
                        for (int i = 0; i < image_width; i++) {
                                double r = ((double) i) / ((double) image_width  - 1);
                                double g = ((double) j) / ((double) image_height - 1);
                                double b = 0.0;

                                Color c = (Color) { (unsigned char)(255.999 * r),
                                                    (unsigned char)(255.999 * g),
                                                    (unsigned char)(255.999 * b),
                                                    255 };
                        
                                DrawPixel(i, j, c);
                        }

                }
                /* asm("int3"); */

                EndDrawing();
        }

        CloseWindow();

        return 0;
}
