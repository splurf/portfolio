#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int BUFLEN = 128; //  over-marginalized length for every buffer

int read_line(char buf[BUFLEN], int *len) {
    char *s = fgets(buf, BUFLEN, stdin);
    if (s == NULL || *s == '\n') {
        return 0;
    } else {
        *len = strlen(buf);
        return 1;
    }
}

int main() {
    char buf[BUFLEN]; // stdin line buffer
    int len; // buflen

    int lengths[BUFLEN]; //  initialize array containing length of words (128 max, kinda over-estimating here)
    memset(&lengths, 0, BUFLEN *sizeof(int)); //  initialize every value as 0

    int min = BUFLEN; //  initialize minimum length ;)
    int max = 0; //  initialize maximum length

    int mode = 0; //  initialize mode
    int mode_idx = 0; //  initialize mode index

    int counter = 0; //  length of current word

    //  continue while a new line of stdin exists
    while (read_line(buf, &len)) {
        for (int i = 0; i < len; i++) {
            char c = buf[i]; //  the current character

            if (c == ' ' || c == '/' || c == '\n') {
                //  in case several of the following conditionals were found in a row
                if (counter == 0) {
                    continue;
                }

                //  determine current min and max
                if (counter < min) {
                    min = counter;
                } else if (counter > max) {
                    max = counter;
                }

                lengths[counter] += 1; // increment value at length

                //  determine current mode
                int len = lengths[counter];
                if (len > mode) {
                    mode = len;
                    mode_idx = counter;
                }
                counter = 0; // reset counter
            } else {
                //  if the current character is a number, letter from alphebet (upper/lower), or dash or apostrophe
                if ((c > 47 && c < 58) || (c > 64 && c < 91) || (c > 96 && 123) || c == 39 || c == 45) {
                    counter += 1; //  increment counter
                }
            }
        }
    }
    printf("word length minimum: %d\nword length maximum: %d\nword length mode: %d\n", min, max, mode_idx);
    return 0;
}