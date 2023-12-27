#include "gfx.h"


/** modify the pixels of `data` to form a sphere */
void sphere(struct Image img, const struct Vec3 center, const float radius, const struct Camera camera) {
    // Sphere parameters
    struct Sphere sphere = { center, radius };

    for (int y = 0; y < img.h; y++) {
        for (int x = 0; x < img.w; x++) {
            // calculate the direction of the ray from the current (x, y) position
            struct Vec3 dir = ray_direction(x, y, img.w, img.h, camera);

            // determine if the ray passes through ("interacts") with the sphere
            float t = sphere_intersect(dir, sphere);

            // `DARK_RED` if the ray interacts with the sphere, `WHITE` otherwise
            struct Color traced = t >= 0 ? WHITE : DARK_RED;

            // modify the pixel at the current position
            img.data[img.w * y + x ] = traced;      
        }
    }
}

int main() {
    const int w = 512; // width
    const int h = 512; // height

    // camera position and size
    const struct Camera camera = {{ -1.0, 1.0, -2.0 }, 2.0, 2.0};

    //  Part 1: Generating Rays
    {
        // the ray directions
        const struct Vec3 ray_1 = ray_direction(0, h - 1, w, h, camera);
        const struct Vec3 ray_2 = ray_direction(w - 1, 0, w, h, camera);
        const struct Vec3 ray_3 = ray_direction(w / 2, h / 2, w, h, camera);

        // Print ray information
        printf("Bottom left pixel\n");
        printf("RayPosition %f %f %f\n", 0.0, 0.0, 0.0);
        printf("RayDirection %f %f %f\n", ray_1.x, ray_1.y, ray_1.z);

        printf("\nTop right pixel\n");
        printf("RayPosition %f %f %f\n", 0.0, 0.0, 0.0);
        printf("RayDirection %f %f %f\n", ray_2.x, ray_2.y, ray_2.z);

        printf("\nMiddle pixel\n");
        printf("RayPosition %f %f %f\n", 0.0, 0.0, 0.0);
        printf("RayDirection %f %f %f\n", ray_3.x, ray_3.y, ray_3.z);
    }
    
    // Part 2: Sphere intersection
    {
        // initiate struct for pixel data
        struct Image img = init(w, h);

        // overwrite pixel data with sphere
        sphere(img, (struct Vec3) { 2.0, 2.0, -16.0 }, 5.3547, camera);

        // write to file as png
        write_to_png("sphere.png", img);

        // `free` the image data
        free(img.data);
    }
    return 0;
}