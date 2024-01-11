#include "gfx-3.h"

void render_pixel(const struct Ray ray, struct Vec3 *color, const int n, const int i) {
    struct RayHit rh; // hit position and direction of normal

    // determine intersection between the ray and any object
    if (closest_object(ray, &rh, i)) {
        if (rh.mat.reflective) {
            // limit to 10 to prevent infinite reflections
            if (n < 10) {
                // reflected across normal
                const struct Vec3 r = reflect(rh.hit, rh.normal);
                const struct Ray r_hit_ray = { r, rh.hit };

                // repeat process for reflected ray
                render_pixel(r_hit_ray, color, n + 1, rh.index);
            }
        } else {
            // calculate the diffuse value
            const float diffuse = calc_diffuse(rh, rh.index);

            // modify the current color value to the diffuse value
            *color = vec_scale(rh.mat.color, diffuse);
        }
    }
}

void render_objects(struct Image img) {
    // color config for each pixel
    struct Vec3 color;

    for (int y = 0; y < img.h; y++) {
        for (int x = 0; x < img.w; x++) {
            color = BLACK; // reset to BLACK

            // calculate the direction of the ray from the current position
            const struct Ray ray = ray_dir(x, y);

            // render everything at this position
            render_pixel(ray, &color, 0, -1);

            // modify the pixel at the current position
            img.data[img.w * y + x] = (struct Color) {
                (int) (255.0f * color.x),
                (int) (255.0f * color.y),
                (int) (255.0f * color.z)
            };
        }
    }
}

int main() {
    // initiate struct for pixel data
    const struct Image img = init(WIDTH, HEIGHT);

    // render everything
    render_objects(img);

    // write to file as png
    write_to_png("reference.png", img);

    // `free` the image data
    free(img.data);

    return 0;
}