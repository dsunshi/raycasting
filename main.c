
#include "raylib.h"
#include "raymath.h"
#include "stdio.h"
#include "stdlib.h"

// TODO: pointer?
Color ray_color(const Ray r);

Color ray_color(const Ray r) {
        Vector3 unit_direction = Vector3Normalize(r.direction);
        double  a = 0.5 * (unit_direction.y + 1.0);
        Vector3 c = Vector3Add( Vector3Scale( Vector3One(), 1.0 - a), Vector3Scale((Vector3){0.5, 0.7, 1.0}, a));
        return ColorFromNormalized((Vector4) {c.x, c.y, c.z, 1.0});
}

int main(void) {
        // Image
        const double aspect_ratio = 16.0 / 9.0;
        const int image_width     = 400;
        // TODO: Need to assert that the image height is at least one
        const int image_height    = (int) (image_width / aspect_ratio);

        // Viewport
        const float viewport_height = 2.0; // Arbitrary
        const float viewport_width  = viewport_height * ((float) (image_width / image_height));

        // Camera (right-handed coordinates)
        // - center at (0, 0, 0)
        // - y-axis goes up
        // - x-axis goes right
        // - negative z-axis pointing in the viewing direction

        const float focal_length = 1.0;
        // TODO: Causes warning - another reason for an init function?
        Vector3 camera_center = Vector3Zero();

        Vector3 viewport_u = (Vector3) {viewport_width, 0.0, 0.0};
        Vector3 viewport_v = (Vector3) {0.0, -viewport_height, 0.0};

        Vector3 pixel_delta_u = Vector3Scale(viewport_u, (float) (1.0 / image_width));
        Vector3 pixel_delta_v = Vector3Scale(viewport_v, (float) (1.0 / image_height));

        /* // Calculate the location of the upper left pixel. */
        Vector3 viewport_upper_left = Vector3Subtract( camera_center,
                                        Vector3Add( (Vector3) {0.0, 0.0, focal_length},
                                          Vector3Add( Vector3Scale(viewport_u, 0.5),
                                                      Vector3Scale(viewport_v, 0.5))));
        Vector3 pixel00_loc = Vector3Add(viewport_upper_left, Vector3Scale( Vector3Add( pixel_delta_u, pixel_delta_v), 0.5));

        InitWindow(image_width, image_height, "Ray Tracing in One Weekend - Listing 9");

        while (!WindowShouldClose()) {
                BeginDrawing();

                for (int j = 0; j < image_height; j++) {
                        for (int i = 0; i < image_width; i++) {
                                Vector3 pixel_center = Vector3Add( pixel00_loc,
                                                       Vector3Add( Vector3Scale(pixel_delta_u, (float) i),
                                                                   Vector3Scale(pixel_delta_v, (float) j))); 
                                Vector3 ray_direction = Vector3Subtract(pixel_center, camera_center);

                                Ray r   = (Ray) {camera_center, ray_direction};
                                Color c = ray_color(r);
                        
                                DrawPixel(i, j, c);
                        }

                }

                EndDrawing();
        }

        CloseWindow();

        return 0;
}
