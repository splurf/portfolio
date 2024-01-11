#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include <inttypes.h>
#include <math.h>


// convenience struct for a vector with 3 components
struct Vec3 {
    float x, y, z;
};

// Define a struct for a Sphere
struct Sphere {
    struct Vec3 center;
    float radius;
};

// convenience struct for pixel-color
struct Color {
    uint8_t r; // red value
    uint8_t g; // green value
    uint8_t b; // blue value
};

// convenience struct containing all image data
struct Image {
    struct Color *data;
    const int w, h;
};

// convenience struct for camera info
struct Camera {
    struct Vec3 pos;
    float w, h;
};

// convenience constants for common colors
const struct Color WHITE = { 255, 255, 255 };
const struct Color DARK_RED = { 128, 0, 0 };


// convenience method for calculating the dot product between two vectors
const float dot(const struct Vec3 u, const struct Vec3 v) {
    return (u.x * v.x) + (u.y * v.y) + (u.z * v.z);
}

// convenience method for normalizing a vector
struct Vec3 normalize(const struct Vec3 u) {
    const float len = sqrtf((u.x * u.x) + (u.y * u.y) + (u.z * u.z));
    return (struct Vec3) {
      u.x / len,
      u.y / len,
      u.z / len  
    };
}

// determine the direction of a ray based on the provided camera and vector position
struct Vec3 ray_direction(const int x, const int y, const int w, const int h, const struct Camera camera) {
    // pixel width and height
    const float pixel_width = camera.w / (float) w;
    const float pixel_height = camera.h / (float) h;

    // pixel offset
    const float x_offset = pixel_width / camera.w;
    const float y_offset = pixel_height / camera.h;

    // calculate e direction of ray
    const struct Vec3 dir = {
        camera.pos.x + ((x * pixel_width) + x_offset),  // x-component
        camera.pos.y - ((y * pixel_height) + y_offset), // y-component
        camera.pos.z                                    // z-component
        };

    // normalize direction vector
    return normalize(dir);
}

// calculate the intersection of a ray and a sphere
float sphere_intersect(struct Vec3 d, struct Sphere sphere) {
    // discriminant coefficient for `d * d`
    const float discriminant = dot(d, d);

    if (discriminant < 0.0) {
        // no interaction
        return -1.0;
    } else {
        //  vector coefficient for `e - c`
        const struct Vec3 c = {
            -sphere.center.x,
            -sphere.center.y,
            -sphere.center.z
            };

        // coefficient for `-d * (e - c)`
        const float a = dot((struct Vec3) { -d.x, -d.y, -d.z }, c);

        // coefficient for `sqrt(...)`
        const float b = sqrtf((a * a) - discriminant * (dot(c, c) - (sphere.radius * sphere.radius)));

        // calculate both solutions
        const float t1 = (a - b) / discriminant;
        const float t2 = (a + b) / discriminant;

        // return the smaller solution
        return fmin(t1, t2);
    }
}

// properly instantiate the `Image` struct with the given dimensions
struct Image init(const int w, const int h) {
    struct Color *data = calloc(w * h, sizeof(struct Color));
    return (struct Image) { data, w, h };
}

// convenience method for writing an `Image` to a png type file
void write_to_png(const char *filename, const struct Image img) {
    stbi_write_png(filename, img.w, img.h, 3, img.data, img.w * 3);
}