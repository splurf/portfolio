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
                lengths[counter] += 1; // increment value at length
                counter = 0; // reset counter
            } else {
                //  if the current character is a apostrophe, hyphen, number (0-9), or letter from alphebet (uppercase/lowercase)
                if (c == 39 || c == 45 || (c > 47 && c < 58) || (c > 64 && c < 91) || (c > 96 && 123)) {
                    counter += 1; //  increment counter
                }
            }
        }
    }
    //  print number of occurrences of each length
    for (int i = 0; i < BUFLEN; i++) {
        int n = lengths[i];
        //  skip over those which had zero occurrences
        if (n > 0) {
            printf("%d-character words: %d\n", i, n);
        }
    }
    return 0;
}