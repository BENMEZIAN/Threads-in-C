#include <stdio.h>
#include <pthread.h>

#define NUM_THREADS 10

// Global flag to signal other threads to exit
volatile int exit_flag = 0;

// Function to check if a number is prime based on remainders
int is_prime(int n) {
    if (n <= 1) {
        return 0; // Not prime
    }
    if (n == 2) {
        return 1; // Prime
    }
    if (n % 2 == 0) {
        return 0; // Not prime
    }
    for (int i = 3; i * i <= n; i += 2) {
        if (n % i == 0) {
            return 0; // Not prime
        }
    }
    return 1; // Prime
}

// Thread function to check primality
void* check_prime(void* arg) {
    int num = *((int*)arg);
    
    if (!is_prime(num)) {
        // Found a divisor; signal other threads to exit
        exit_flag = 1;
        printf("%d is not a prime (divisor found)\n", num);
    } else {
        // Number is still potentially prime; continue checking
        printf("%d is prime\n", num);
    }
    
    pthread_exit(NULL);
}

int main() {
	
    int numbers[] = {2, 3, 4, 5, 6, 7, 8, 9, 10};
    pthread_t threads[NUM_THREADS];
    int results[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, check_prime, &numbers[i]);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], (void**)&results[i]);
    }

    return 0;
}
