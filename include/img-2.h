#ifndef IMAGE_H_
#define IMAGE_H_

/** Convenience structure containing the necessary information for an image */
struct Image {
    unsigned int w, h;     // width and height
    unsigned int **pixels; // uninitialized pointer to (pointer-to-integer)
};

/** Constructor method for Image to create a new image based on the provided width and height */
struct Image create_image(unsigned int w, unsigned int h);

/** Deconstructor method for Image */
void destroy_image(struct Image img);

/** Convenience function for printing an error */
unsigned int err(const char *c, ...);

/** Split the provided string into an array of size 2 based on the provided delimiter */
unsigned int split_once(char *arg, char delim, char *split[2]);

/** Parse the provided string to an integer*/
unsigned int to_int(char *s, unsigned int *ptr);

/** Specific parsing to the initial argument */
unsigned int parse_initial_arg(unsigned int argc, char **argv, unsigned int *w, unsigned int *h);

/** Parse the provided individual argument */
unsigned int parse_arg(unsigned int **pixels, char *arg, unsigned int *len, unsigned int *full, unsigned int *r, unsigned int *c, unsigned int *p);

/** Parse the provided arguments */
unsigned int parse_args(struct Image img, unsigned int argc, char **argv);

/** Format the contents of the provided image then write to stdout */
int write_pgm(struct Image img);

#endif