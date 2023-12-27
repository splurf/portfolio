#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

struct Pixel {
    int r, c, p;
};

int split_once(char *arg, char delim, char *split[2]) {
    char *e = strchr(arg, delim);
    if (e == NULL) {
        return 0;
    }
    int i = (int)(e - arg);
    int len = strlen(arg);
    char *current;

    current = (char *)malloc(i);
    strncpy(current, arg, i);
    split[0] = current;
    current = (char *)malloc(len - i);
    strncpy(current, arg + i + 1, len - i);
    split[1] = current;
    return 1;
}

int to_int(char *s, int *ptr) {
    // check if every character is a valid digit
    for (int i = 0; i < (int)strlen(s); i++) {
        if (!isdigit(s[i])) {
            return 0;
        }
    }
    *ptr = atoi(s);
    return 1;
}

int parse_initial_arg(int argc, char **argv, int *w, int *h) {
    if (argc <= 1) {
        printf("Missing arguments\n");
        return 0;
    }
    char *size[2] = {};

    if (!split_once(argv[1], 'x', size)) {
        printf("Error: Invalid initial argument\n");
        return 0;
    }
    if (!to_int(size[0], w) || !to_int(size[1], h)) {
        printf("Error: Invalid size\n");
        return 0;
    }
    if (*w > 100) {
        printf("Error: Invalid width\n");
        return 0;
    } else if (*h > 100) {
        printf("Error: Invalid height\n");
        return 0;
    } else {
        return 1;
    }
}

int parse_arg(char *arg, struct Pixel pixels[], int *len, int *full) {
    char *split[2];

    if (!split_once(arg, '=', split)) {
        return to_int(arg, full) == 1 ? 2 : 0;
    } else {
        char *raw_coords[2];
        if (!split_once(split[0], ',', raw_coords)) {
            return 0;
        }
        int r, c, p;
        if (!to_int(raw_coords[0], &r) || !to_int(raw_coords[1], &c) || !to_int(split[1], &p)) {
            return 0;
        }
        struct Pixel pixel = {r, c, p};
        pixels[(*len)++] = pixel;
    }
    return 1;
}

int parse_args(int w, int h, int pgm[h][w], int *len, struct Pixel pixels[], int argc, char **argv) {
    int i = 2;
    int full = 0;

    for (; i < argc; i++) {
        switch (parse_arg(argv[i], pixels, len, &full)) {
        case 0:
            printf("Error: Invalid argument\n");
            return 0;
        case 1:
            {
                struct Pixel p = pixels[*len - 1];
                if (p.r > h) {
                    printf("Error: Pixel row out of range (0-%d)\n", h);
                    return 0;
                } else if (p.c > w) {
                    printf("Error: Pixel column out of range (0-%d)\n", w);
                    return 0;
                } else if (p.p < 0 || p.p > 255) {
                    printf("Error: Pixel value out of range (0..255)\n");
                    return 0;
                }
            };
            break;
        case 2:
            for (int r = 0; r < h; r++) {
                for (int c = 0; c < w; c++) {
                    pgm[r][c] = full;
                }
            }
            break;
        default:
            break;
        }
    }
}

void adjust_pgm(int w, int h, int pgm[h][w], int len, struct Pixel pixels[]) {
    for (int i = 0; i < len; i++) {
        struct Pixel p = pixels[i];
        pgm[p.r][p.c] = p.p;
    }
}

void print_pgm(int w, int h, int pgm[h][w]) {
    printf("P2\n%d %d\n255\n", w, h);
    for (int r = 0; r < h; r++) {
        for (int c = 0; c < w; c++) {
            printf("%d", pgm[r][c]);
            if (c < w - 1) {
                printf("\t");
            }
        }
        printf("\n");
    }
}

int main(int argc, char **argv) {
    int w, h; // based on the initial argument
    if (!parse_initial_arg(argc, argv, &w, &h)) {
        return 1;
    };
    int pgm[h][w];

    int len = 0;
    struct Pixel pixels[argc];

    if (!parse_args(w, h, pgm, &len, pixels, argc, argv)) {
        return 1;
    }
    adjust_pgm(w, h, pgm, len, pixels);
    print_pgm(w, h, pgm);
    return 0;
}

