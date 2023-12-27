#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

//  unsigned 8-bit integer
typedef unsigned char uint_8;

// convenience struct for pixel-color
struct Color {
    uint_8 r; // red value
    uint_8 g; // green value
    uint_8 b; // blue value
};

struct Image {
    struct Color *data;
    const int w, h;
};

struct Image init(const int w, const int h) {
    struct Color *data = calloc(w * h, sizeof(struct Color));
    return (struct Image) { data, w, h };
}

void fill(struct Image img, struct Color color) {
    for (int i = 0; i < img.w * img.h; i++) {
        img.data[i] = color;
    }
}

void write_to_png(const char *filename, const struct Image img) {
    stbi_write_png(filename, img.w, img.h, 3, img.data, img.w * 3);
}