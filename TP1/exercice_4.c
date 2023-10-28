#include <stdio.h>
#include <time.h>
#include <stdint.h>
#include <pthread.h>

#define DIM 4
#define NUM_THREADS 4

long matrix_a[DIM][DIM];
long matrix_b[DIM][DIM];
long matrix_c[DIM][DIM];

long shared_sum = 0; // Variable shared by all threads for sum
pthread_mutex_t sum_lock; // Mutex for sum synchronization

void init();
void *multiply(void *arg);
void compute_sum(int start_row, int end_row);
void print();


int main(void) {
    
	pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];
    
    time_t t1 = time(NULL);
    init();
    time_t t2 = time(NULL);
    
    printf("Time of init function = %ld seconds\n", (t2 - t1));
    
    time_t t3 = time(NULL);
    
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, multiply, &thread_ids[i]);
    }
    
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    
    time_t t4 = time(NULL);
    
    printf("Time of multiply function = %ld seconds\n", (t4 - t3));
    
    // Calculate the sum in parallel
    compute_sum(0, DIM);
    
    // Print the sum
    printf("Sum of matrix_c = %ld\n", shared_sum);
    
    print();
    
    
    return 0;
}

void init() {
    
	for (int i = 0; i < DIM; i++) {
        for (int j = 0; j < DIM; j++) {
            matrix_a[i][j] = i + j;
            matrix_b[i][j] = i - j;
            matrix_c[i][j] = 0;
        }
    }
}

void *multiply(void *arg) {
	
    int thread_id = *((int *)arg);
    int start_row = thread_id * (DIM / NUM_THREADS);
    int end_row = (thread_id + 1) * (DIM / NUM_THREADS);
    
    printf("I'm thread %d.\n", thread_id);
    
    for (int i = start_row; i < end_row; i++) {
        for (int j = 0; j < DIM; j++) {
            for (int k = 0; k < DIM; k++) {
                matrix_c[i][j] += matrix_a[i][k] * matrix_b[k][j];
            }
        }
    }

    printf("Thread %d finished.\n", thread_id);
    return NULL;
}

void compute_sum(int start_row, int end_row) {
	
    long local_sum = 0;
    
    for (int i = start_row; i < end_row; i++) {
        for (int j = 0; j < DIM; j++) {
            local_sum += matrix_c[i][j];
        }
    }

    pthread_mutex_lock(&sum_lock);  // Lock the mutex before updating the shared sum
    shared_sum += local_sum;
    pthread_mutex_unlock(&sum_lock);  // Unlock the mutex
}

void print() {
	
    FILE *fp = fopen("parallel.txt", "w");

    for (int i = 0; i < DIM; i++) {
        for (int j = 0; j < DIM; j++) {
            fprintf(fp, " %ld", matrix_c[i][j]);
        }
        fprintf(fp, "\n");
    }
    fclose(fp);
}
