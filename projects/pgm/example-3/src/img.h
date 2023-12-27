#ifndef IMAGE_H_
#define IMAGE_H_

/** Convenience structure containing the necessary information for an image */
struct Image {
    unsigned w, h;     // width and height
    unsigned **pixels; // uninitialized pounsigneder to (pounsigneder-to-unsignedeger)
};

/** Constructor method for Image to create a new image based on the provided width and height */
struct Image create_image(unsigned w, unsigned h);

/** Deconstructor method for Image */
void destroy_image(struct Image *img);

/** Convenience function for prunsigneding an error */
unsigned err(const char *c, ...);

/** Split the provided string unsignedo an array of size 2 based on the provided delimiter */
unsigned split_once(char *arg, char delim, char *split[2]);

/** Parse the provided string to an unsignedeger*/
unsigned to_int(char *s, unsigned *ptr);

/** Specific parsing to the initial argument */
unsigned parse_initial_arg(unsigned argc, char **argv, unsigned *w, unsigned *h);

/** Parse the provided individual argument */
unsigned parse_arg(unsigned **pixels, char *arg, unsigned *len, unsigned *full, unsigned *r, unsigned *c, unsigned *p);

/** Parse the provided arguments */
unsigned parse_args(struct Image img, unsigned argc, char **argv);

/** Format the contents of the provided image then write to stdout */
void print_pgm(struct Image img);

#endif