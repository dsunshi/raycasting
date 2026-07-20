
#include "raylib.h"
#include "raymath.h"
/* #include "stdio.h" */
/* #include "stdlib.h" */

#define USE_RAY_MATH false

// https://en.wikipedia.org/wiki/Ray_tracing_(graphics)#/media/File:Ray_Tracing_Illustration_First_Bounce.png
// TODO: pointer?
Color ray_color(const Ray r);
    /* point3 at(double t) const { */
    /*     return orig + t*dir; */
    /* } */
Vector3 at(Ray r, float t);

Vector3 at(Ray r, float t) {
        return Vector3Add(r.position, Vector3Scale(r.direction, t));
}

float hit_sphere(const Vector3 center, float radius, const Ray r);

float hit_sphere(const Vector3 center, float radius, const Ray r) {

        const Vector3 oc = Vector3Subtract(center, r.position);
        float a = Vector3LengthSqr(r.direction);
        float h = Vector3DotProduct(r.direction, oc);
        float c = Vector3LengthSqr(oc) - radius * radius;
        float discriminant = h * h - a * c;

        if (discriminant < 0) {
                return -1.0;
        } else {
                return (h - sqrtf(discriminant) ) / a;
        }
}

Color ray_color(const Ray r) {
        Vector3 co   = (Vector3) {0.0, 0.0, -1.0};
        float radius = 0.5;
        
#if USE_RAY_MATH
        RayCollision rc = GetRayCollisionSphere(r, co, radius);
        if (rc.hit) {
                Vector3 at = Vector3Add(r.position, Vector3Scale(r.direction, -1.0));
                /* Vector3 n  = Vector3Normalize(Vector3Subtract(at, (Vector3) {0.0, 0.0, -1.0})); */
                /* return ColorFromNormalized((Vector4) {(n.x + 1.0)/2.0, (n.y + 1.0)/2.0, (n.z + 1.0)/2.0, 1.0}); */
                /* Vector3 normal   = (Vector3) { 0.0f, 0.0f, -1.0f }; */
                Vector3 view_dir = Vector3Normalize(Vector3Subtract(r.direction, r.position));

                /* float dot = Vector3DotProduct(normal, view_dir); */
                float dot = Vector3DotProduct(rc.normal, view_dir);
                if (dot > 0.0f) {
                        return ColorFromNormalized((Vector4) {(view_dir.x + 1.0)/2.0, (view_dir.y + 1.0)/2.0, (view_dir.z + 1.0)/2.0, 1.0});
                }
        }
#else
        float t = hit_sphere(co, radius, r);
        if (t > 0.0) {
                Vector3 N = Vector3Normalize(Vector3Subtract(at(r, t), co));
                return ColorFromNormalized((Vector4) {(N.x + 1.0)/2.0, (N.y + 1.0)/2.0, (N.z + 1.0)/2.0, 1.0});
        }
#endif
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
        const float viewport_height = 3.0; // Arbitrary (higher number "zooms out"
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
        // TODO: Why the `/ aspect_ratio`?
        Vector3 viewport_v = (Vector3) {0.0, -viewport_height / aspect_ratio, 0.0};

        Vector3 pixel_delta_u = Vector3Scale(viewport_u, (float) (1.0 / image_width));
        Vector3 pixel_delta_v = Vector3Scale(viewport_v, (float) (1.0 / image_height));

        /* // Calculate the location of the upper left pixel. */
        // TODO I think these should be the same? Maybe some kind of check?
        /* Vector3 viewport_upper_left = Vector3Subtract( camera_center, */
        /*                                 Vector3Add( (Vector3) {0.0, 0.0, focal_length}, */
        /*                                   Vector3Add( Vector3Scale(viewport_u, 0.5), */
        /*                                               Vector3Scale(viewport_v, 0.5)))); */
        Vector3 viewport_upper_left = Vector3Subtract(
                                      Vector3Subtract(
                                      Vector3Subtract(camera_center, (Vector3) {0.0, 0.0, focal_length}),Vector3Scale(viewport_u, 0.5)), Vector3Scale(viewport_v, 0.5));
        Vector3 pixel00_loc = Vector3Add(viewport_upper_left, Vector3Scale( Vector3Add( pixel_delta_u, pixel_delta_v), 0.5));

        InitWindow(image_width, image_height, "Ray Tracing in One Weekend - Image 4: A sphere colored according to its normal vectors");

        while (!WindowShouldClose()) {
                BeginDrawing();

                for (int j = 0; j < image_height; j++) {
                        for (int i = 0; i < image_width; i++) {
                                Vector3 pixel_center = Vector3Add( pixel00_loc,
                                                       Vector3Add( Vector3Scale(pixel_delta_u, (float) i),
                                                                   Vector3Scale(pixel_delta_v, (float) j))); 
                                Vector3 ray_direction = Vector3Subtract(pixel_center, camera_center);

#if USE_RAY_MATH
                                Ray r   = (Ray) {ray_direction, camera_center};
#else
                                Ray r   = (Ray) {camera_center, ray_direction};
#endif
                                Color c = ray_color(r);
                        
                                DrawPixel(i, j, c);
                        }

                }

                EndDrawing();
        }

        CloseWindow();

        return 0;
}
