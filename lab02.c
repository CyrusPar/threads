#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

// Structure to pass data to threads
typedef struct {
    int **X;
    int *y;
    int *p;
    int n;
    int scol; //start column
    int ecol;  //end column
} ThreadData;

//transpose the matrix
void transpose(int **X, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < i; j++) {
            int temp = X[i][j];  //swap
            X[i][j] = X[j][i];
            X[j][i] = temp;
        }
    }
}

//thread execution
void *dp(void *arg) {
    ThreadData *data = (ThreadData *)arg; //access the arguments
    for (int j = data->scol; j < data->ecol; j++) { 
        int sum = 0;
        for (int i = 0; i < data->n; i++) {
            sum += data->y[i] * data->X[i][j]; //do the dot product
        }
        data->p[j] = sum; 
    }
    pthread_exit(NULL);
}

int main() {
    int n, t;
    printf("Enter the value of n: "); 
    scanf("%d", &n);
    printf("Enter the number of threads (t): ");
    scanf("%d", &t);

    //allocate memmory
    int **X = (int **)malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++) {
        X[i] = (int *)malloc(n * sizeof(int));
    }
    int *y = (int *)malloc(n * sizeof(int));
    int *p = (int *)malloc(n * sizeof(int));

    srand(time(NULL));

    
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            X[i][j] = (rand() % 20) + 1; //generate non zero for matrix x
        }
    }
    for (int i = 0; i < n; i++) {  //generate non zero for vectory y
        y[i] = (rand() % 20) + 1;
    }

    //call the transpose function
    transpose(X, n);

    //before
    clock_t time_before = clock();

    //create threads
    pthread_t threads[t];
    ThreadData thread_data[t];
    int chunk = n / t;

    for (int i = 0; i < t; i++) {
        thread_data[i].X = X;
        thread_data[i].y = y;
        thread_data[i].p = p;
        thread_data[i].n = n;
        thread_data[i].scol = i * chunk;
        thread_data[i].ecol = (i == t - 1) ? n : (i + 1) * chunk;
        pthread_create(&threads[i], NULL, dp, &thread_data[i]); //call the thread function dp and pass the arguments
    }

    //join threads
    for (int i = 0; i < t; i++) {
        pthread_join(threads[i], NULL);
    }

    //after
    clock_t time_after = clock();

    //elapsed time
    double time_elapsed = ((double)(time_after - time_before)) / CLOCKS_PER_SEC;
    printf("Time elapsed: %.6f seconds\n", time_elapsed);

    // Free memory
    for (int i = 0; i < n; i++) {
        free(X[i]);
    }
    free(X);
    free(y);
    free(p);

    return 0;
}
