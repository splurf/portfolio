#include <stdio.h>

#define N 8 // array len

void print_array(const int arr[N]) {
    printf("{");
    for (int i = 0; i < N; i++) {
        printf("%d", arr[i]);
        if (i < N - 1) {
            printf(", ");
        }
    }
    printf("}\n");
}

/** Qustion 2 (v1)
 *
 * achieves the same result as the MIPS code
 */
void mips_as_c(int s0, int s1, int *s6, int *s7) {
    int sum = 0;
    for (int i = s0; i < s0 + 2; i++) {
        sum += s6[i];
    }
    s7[s1] = sum;
}

/** Qustion 2 (v2)
 *
 * direct conversion of the MIPS code
 */
void mips_to_c(int s0, int s1, int *s6, int *s7) {
    // calculate the indices for s6 and s7 using bit-shifts
    int t0 = s0 << 2;
    int t1 = s1 << 2;

    // access elements in s6 and s7
    int s0_value = s6[t0 >> 2];
    int t2 = t0 + 4;
    int t0_value = s6[t2 >> 2];
    int t0_final = t0_value + s0_value;

    // store t0_final in s7
    s7[t1 >> 2] = t0_final;
}

int main() {
    // define the arrays s6 and s7
    int s6[N] = {1, 2, 3, 4, 5, 6, 7, 8};
    int s7[N] = {0, 0, 0, 0, 0, 0, 0, 0};

    // load immediate values into s0 and s1
    int s0 = 2;
    int s1 = 5;

    // mips_as_c(s0, s1, s6, s7);
    mips_to_c(s0, s1, s6, s7);

    // print the modified s7
    print_array(s7);
}