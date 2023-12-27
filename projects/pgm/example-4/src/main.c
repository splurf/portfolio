#include "img.h"
#include <stdio.h>


int main(int argc, char **argv) {
    int w, h; // based on the initial argument

    // parse the initial argument
    if (!parse_initial_arg(argc, argv, &w, &h))
        return 1;

    // allocate a new image with the specified width and height
    struct Image img = create_image(w, h);

    // parse the rest of the arguments
    int rc = parse_args(img, argc, argv);

    if (rc)
        write_pgm(img);

    // free the image nevertheless
    destroy_image(img);

    // return the correct return code accordingly
    return rc ? 0 : 1;
}