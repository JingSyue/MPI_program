#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void sieve_of_eratosthenes(int n) {
    char* is_prime = malloc((n+1) * sizeof(char));
    memset(is_prime, 1, n+1);

    for (int p = 2; p*p <= n; p++) {
        if (is_prime[p]) {
            for (int i = p*p; i <= n; i += p) {
                is_prime[i] = 0;
            }
        }
    }

    // Example for printing primes
    for (int p = 2; p <= n; p++) {
        if (is_prime[p]) {
            printf("%d ", p);
        }
    }

    free(is_prime);
}

int main() {
    int n=60;
    clock_t start = clock();
    sieve_of_eratosthenes(n);
    clock_t end = clock();
    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
    printf("\nTime spent: %f\n", time_spent);
    return 0;
}
