#include <stdio.h>
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

    int counter = 0; // number of total words

    while (read_line(buf, &len)) {
        //  iterate through each character within the character array
        for (int i = 0; i < len; i++) {
            char c = buf[i]; //  the current character
            //  if word-separating character
            if (c == ' ' || c == '/' || c == '\n') {
                counter += 1; //  increment word counter
            }
        }
    }
    printf("number of words: %d\n", counter);
    return 0;
}