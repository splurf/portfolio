# concurrency

This is a simple example showcasing how [mutexes](https://en.wikipedia.org/wiki/Lock_(computer_science)) ([mutal exclusion](https://en.wikipedia.org/wiki/Mutual_exclusion)) and [condition variables](https://en.wikipedia.org/wiki/Monitor_(synchronization)#Condition_variables) work together.

## Description
There are two threads. The main thread handles STDIN, while other thread handles STDOUT. The program itself is essentially always paused, resulting in incredibly low CPU usage. The main thread pauses its thread by waiting for the user's input. The other thread stays paused as long as the main thread hasn't received any input. After an input is recognized, the main thread handles that, increments the counter (if applicable), then signals the condition variable which the other thread is waiting on, then continues to pause its thread. On the other hand, the other thread resumes as its condition has been passed, prints the counter to STDOUT, then signals the condition variable. That's it. Besides some locking/unlocking before and after the signaling, both threads are essentially just trading places (running/not running) whenever an input is detected. Although not the most practical example, it showcases the use of multi-threading by utilizing mutexes and condition variables.

## Usage
This program prompts the user for an input. If the user enters "exit", the program will properly end. If an number is provided, it will add that value to a globally established counter. That's it.