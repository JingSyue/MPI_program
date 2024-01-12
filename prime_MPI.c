#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_RANGE_SIZE 1000000

void parallel_sieve(int start, int end, int rank, int size) {
    char is_prime[MAX_RANGE_SIZE];
    memset(is_prime, 1, sizeof(is_prime));

    int global_sqrt_n = (int)sqrt((double)MAX_RANGE_SIZE) + 1;
    char prime_sqrt_n[global_sqrt_n];
    memset(prime_sqrt_n, 1, global_sqrt_n);

    if (rank == 0) {
        for (int k = 2; k * k <= global_sqrt_n; k++) {
            if (prime_sqrt_n[k]) {
                for (int i = k * k; i <= global_sqrt_n; i += k) {
                    prime_sqrt_n[i] = 0;
                }
            }
        }
    }

    // Broadcast the prime list up to global_sqrt_n to all processes
    MPI_Bcast(prime_sqrt_n, global_sqrt_n, MPI_CHAR, 0, MPI_COMM_WORLD);

    // Each process marks non-primes in its range
    for (int k = 2; k <= global_sqrt_n; k++) {
        if (prime_sqrt_n[k]) {
            int min_multiple = (int)fmax(k * k, start); // Ensure starting from the correct multiple
            if (min_multiple % k != 0) {
                min_multiple += (k - (min_multiple % k)); // Adjust to the next multiple of k
            }
            for (int j = min_multiple; j <= end; j += k) {
                is_prime[j - start] = 0; // Mark as non-prime
            }
        }
    }

    // Print primes in each process's range
    for (int p = start; p <= end; p++) {
        if (is_prime[p - start]) {
            printf("Process %d: %d\n", rank, p);
        }
    }
}

int main(int argc, char** argv) {
    int n;
    int rank, size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (argc > 1) {
        n = atoi(argv[1]);
    } else {
        n = MAX_RANGE_SIZE;
    }

    int segment = n / size;
    int local_start = 2 + rank * segment;
    int local_end = (rank == size - 1) ? n : local_start + segment - 1;

    printf("Process %d: local_start = %d, local_end = %d\n", rank, local_start, local_end);

    double start_time = MPI_Wtime();
    parallel_sieve(local_start, local_end, rank, size);
    double end_time = MPI_Wtime();
    if (rank == 0) {
        printf("Total time spent: %f\n", end_time - start_time);
    }

    MPI_Finalize();
    return 0;
}
