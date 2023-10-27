#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 5

int arr[] = {1, 2, 3, 4, 5};
pthread_t threads[NUM_THREADS];
int thread_ids[NUM_THREADS];
int result[NUM_THREADS];

// Function to calculate the square of an individual element
void* square(void* arg) {
    int thread_arg = *((int*)arg);
    int* carre = (int*)malloc(sizeof(int));
    *carre = thread_arg * thread_arg;
    pthread_exit((void*)carre);
}

// Function to calculate the square of an element in an array
void* square_array(void* arg) {
    int thread_id = *((int*)arg); // sending the ID of the thread (i= 0.......NUM_THREADS-1)
    pthread_t tid = pthread_self(); // Get the ID of the current thread
    printf("Thread %d (ID: %p) is calculating the square of arr[%d]\n", thread_id + 1, (unsigned long)tid, thread_id);
    result[thread_id] = arr[thread_id] * arr[thread_id];
    pthread_exit(NULL);
}

int main() {
	
    printf("*************1- Square of one element:*****************\n");
    int arg = 5;
    int* y; // to receive the output

    pthread_t tid;
    pthread_create(&tid, NULL, square, (void*)&arg); // Create a new thread and pass the argument
    pthread_join(tid, (void**)&y); // Wait for the thread to finish

    printf("Square of %d is: %d\n", arg, *y); // Print the result
    free(y); // Free the memory allocated for y

    printf("*************2- Square of an array:********************\n");
    printf("Original array: ");
    for (int i = 0; i < NUM_THREADS; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    for (int i = 0; i < NUM_THREADS; i++) {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, square_array, (void*)&thread_ids[i]);
    }

    // Wait for all threads to finish
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("Squares of elements: ");
    for (int i = 0; i < NUM_THREADS; i++) {
        printf("%d ", result[i]);
    }
    printf("\n");

    return 0;
}
