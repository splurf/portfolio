#include "img.h"
#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/** Constructor method for Image to create a new image based on the provided width and height */
struct Image create_image(unsigned int w, unsigned int h) {
    unsigned int **pixels = malloc(h * sizeof(unsigned int *));

    //  initialize each column to 0
    for (unsigned int r = 0; r < h; r++)
        pixels[r] = calloc(w, sizeof(int));

    struct Image img = {w, h, pixels};
    return img;
}

/** Deconstructor method for Image */
void destroy_image(struct Image *img) {
    free(img->pixels);
}

/** Convenience function for printing an error */
unsigned int err(const char *c, ...) {
    va_list args;
    va_start(args, c);
    fprintf(stderr, "Error: ");
    vfprintf(stderr, c, args);
    fprintf(stderr, "\n");
    va_end(args);
    return 0;
}

/** Split the provided string into an array of size 2 based on the provided delimiter */
unsigned int split_once(char *arg, char delim, char *split[2]) {
    char *e = strchr(arg, delim);
    if (e == NULL)
        return 0;
    unsigned int i = (int)(e - arg);
    unsigned int len = strlen(arg);
    char *current;
    current = (char *)malloc(i);
    strncpy(current, arg, i);
    split[0] = current;
    current = (char *)malloc(len - i);
    strncpy(current, arg + i + 1, len - i);
    split[1] = current;
    return 1;
}

/** Parse the provided string to an integer*/
unsigned int to_int(char *s, unsigned int *ptr) {
    // check if every character is a valid digit
    for (unsigned int i = 0; i < (int)strlen(s); i++)
        if (!isdigit(s[i]))
            return 0;
    *ptr = atoi(s);
    return 1;
}

/** Specific parsing to the initial argument */
unsigned int parse_initial_arg(unsigned int argc, char **argv, unsigned int *w, unsigned int *h) {
    if (argc <= 1)
        return err("Missing arguments");

    char *size[2] = {0};

    if (!split_once(argv[1], 'x', size))
        return err("Invalid initial argument");
    else if (!to_int(size[0], w) || !to_int(size[1], h))
        return err("Invalid size");
    else if (*w > 100)
        return err("Invalid width");
    else if (*h > 100)
        return err("Invalid height");
    else
        return 1;
}

/** Parse the provided individual argument */
unsigned int parse_arg(unsigned int **pixels, char *arg, unsigned int *len, unsigned int *full, unsigned int *r, unsigned int *c, unsigned int *p) {
    char *split[2];

    if (!split_once(arg, '=', split))
        return to_int(arg, full) == 1 ? 2 : 0;
    else {
        char *raw_coords[2];
        if (!split_once(split[0], ',', raw_coords)) {
            return 0;
        } else if (!to_int(raw_coords[0], r) || !to_int(raw_coords[1], c) || !to_int(split[1], p)) {
            return 0;
        } else {
            return 1;
        }
    }
}

/** Parse the provided arguments */
unsigned int parse_args(struct Image img, unsigned int argc, char **argv) {
    unsigned int r, c, p, len = 0, full = 0, i = 2;

    for (; i < argc; i++) {
        switch (parse_arg(img.pixels, argv[i], &len, &full, &r, &c, &p)) {
        case 0:
            return err("Invalid argument");
        case 1: {
            if (r >= img.h)
                return err("Pixel row out of range (0..%d)", img.h);
            else if (c >= img.w)
                return err("Pixel column out of range (0..%d)", img.w);
            else if (p < 0 || p > 255)
                return err("Pixel value out of range (0..255)");
        };
            img.pixels[r][c] = p;
            break;
        case 2:
            if (full < 0 || full > 255)
                return err("Pixel value out of range (0..255)");

            for (unsigned int r = 0; r < img.h; r++)
                for (unsigned int c = 0; c < img.w; c++)
                    img.pixels[r][c] = full;
            break;
        }
    }
    return 1;
}

/** Format the contents of the provided image then write to stdout */
void print_pgm(struct Image img) {
    printf("P2\n%d %d\n255\n", img.w, img.h);
    for (unsigned int r = 0; r < img.h; r++) {
        for (unsigned int c = 0; c < img.w; c++) {
            printf("%d", img.pixels[r][c]);
            if (c < img.w - 1)
                printf("\t");
        }
        printf("\n");
    }
}