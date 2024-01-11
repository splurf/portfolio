#include "gfx-1.h"

/** modify the pixels of `data` to form a checkerboard */
void checkerboard(struct Image img, const int n) {
    const int x_interval = img.w / n; // split the number of x-iterations
    const int y_interval = img.h / n; // split the number of y-iterations

    for (int y = 0; y < y_interval; y++) {
        int i = y * n; // current y position
        const int j = i + n; // where y will end

        for (; i < j; i++) {
            for (int x = 0; x < x_interval; x++) {
                int k = x * n; // current x position
                const int l = k + n; // where x will end

                // write each pixel to `data` in this segment
                for (; k < l; k++) {
                    // alternate columns giving that "checkerboard" effect
                    if (x % 2 == (y % 2 == 1)) {
                        img.data[img.w * i + k].r = 255;
                    } else {
                        img.data[img.w * i + k].b = 255;
                    }
                }
            }
        }
    }
}

/** modify the pixels of `data` to form a gradient */
void gradient(struct Image img) {
    // because there are only 256 bits for each color channel,
    // we need to have 256 segments for both directions
    const int n_w = img.w / 256; // determine segment interval for x direction
    const int n_h = img.h / 256; // determine segment interval for y direction

    const int x_interval = img.w / n_w; // split the number of x-iterations
    const int y_interval = img.h / n_h; // split the number of x-iterations

    //  initial color throughout the gradient
    struct Color color = (struct Color){0, 0, 128};

    for (int y = 0; y < y_interval; y++) {
        int i = y * n_h; // current y position
        const int j = i + n_h; // where y will end

        for (; i < j; i++) {
            for (int x = 0; x < x_interval; x++) {

                int k = x * n_w; // current x position
                const int l = k + n_w; // where x will end

                // write each pixel to `data` in this segment
                for (; k < l; k++) {
                    img.data[img.w * i + k] = color;
                }
                // increment red value along with x position
                color.r += 1;
            }
        }
        // increment green value along with y position
        color.g += 1;
    }
}

int main() {
    const int w = 512; // width
    const int h = 512; // height

    // define array of pixels
    struct Image img = init(w, h);

    // overwrite the necessary pixels of `data` to show a checkerboard
    checkerboard(img, 64);
    write_to_png("checkerboard.png", img);

    // overwrite the necessary pixels of `data` to show a gradient
    gradient(img);
    write_to_png("gradient.png", img);
}