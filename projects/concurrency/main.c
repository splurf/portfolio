#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <pthread.h>

static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER; // Mutually exclusive variable (mutex)
static int counter = 0;                                  // Integer value associated with the mutex

static pthread_cond_t gate = PTHREAD_COND_INITIALIZER; // Condition variable (cond var)
static bool is_open;                                   // Boolean predicate associated with the cond var

/// @brief Block the current thread until the boolean predicate of the condition variable is met.
/// @param _is_open Boolean predicate option of the cooresponding condition variable.
void wait(const bool _is_open) {
    pthread_mutex_lock(&lock); // Aqcuire the lock
    is_open = _is_open;        // Open/Close the gate

    // Wait for the the gate to be opened/closed
    while (_is_open ? is_open : !is_open)
        pthread_cond_wait(&gate, &lock);

    // Release the lock
    pthread_mutex_unlock(&lock);
}

/// @brief Method for external thread with the sole purpose of printing the current value of the counter.
/// @param id Thread id.
static void *routine_handler(void *id) {
    for (;;) {
        // 1. Clear the screen
        // 2. Position cursor to home
        // 3. Print the value of the counter
        // 4. Print some eye candy
        printf("\x1b[2J\x1b[H%d\n> ", counter);

        // Notify other blocked threads to continue
        pthread_cond_signal(&gate);

        // Open the gate, then wait until it's closed
        wait(true);
    }
}

/// @brief Handler for the user input.
void input_handler() {
    const int N = 16; // Size of input buffer
    char input[N];    // Character buffer
    int value;        // Input value as integer

    for (;;) {
        // Read user input
        if (fgets(input, N, stdin) != NULL) {
            // Checks if user wants to exit
            if (strncmp(input, "exit", 4) == 0)
                break;

            // Increment counter with user-provided integer value
            if (sscanf(input, "%d", &value) == 1)
                counter += value;
        }

        // Notify the other thread
        pthread_cond_signal(&gate);

        // Close the gate, then wait until it's open
        wait(false);
    }
}

int main() {
    // Instantiate external thread
    pthread_t routine_thread;
    pthread_create(&routine_thread, NULL, routine_handler, NULL);

    // Close the gate, then wait until it's open
    wait(false);

    // Begin handling input
    input_handler();

    // Clean up
    pthread_cancel(routine_thread); // Kill external thread
    pthread_mutex_destroy(&lock);   // Free mutex
    pthread_cond_destroy(&gate);    // Free cond var

    return 0;
}