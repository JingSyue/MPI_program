#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
void loop_reordering(void){
    time_t start_time, end_time;
    clock_t start, end;
    double elapsed_time, cpu_time;
    int m = 500;  // Rows in matrix A and C
    int n = 500;  // Columns in matrix A and rows in matrix B
    int p = 500;  // Columns in matrix B and C
    double** A, ** B, ** C;
    int i, j, k;
    double f;

    // Allocate memory for matrices A, B, and C
    A = (double**)malloc(m * sizeof(double*));
    B = (double**)malloc(n * sizeof(double*));
    C = (double**)malloc(m * sizeof(double*));
    for (i = 0; i < m; i++) {
        A[i] = (double*)malloc(n * sizeof(double));
        C[i] = (double*)malloc(p * sizeof(double));
    }
    for (i = 0; i < n; i++) {
        B[i] = (double*)malloc(p * sizeof(double));
    }

    start_time = time(NULL);
    start = clock(); // Record the start time

    // Initialize matrices A and B with double-precision values
    for (i = 0; i < m; i++) {
        for (j = 0; j < n; j++) {
            A[i][j] = 1.1;
        }
    }

    for (i = 0; i < n; i++) {
        for (j = 0; j < p; j++) {
            B[i][j] = 1.1;
        }
    }

    

    // Matrix multiplication with loop reordering
    for (i = 0; i < m; i++) {
        for (k = 0; k < n; k++) {
            f = A[i][k];
            for (j = 0; j < p; j++) {
                C[i][j] += f * B[k][j];
            }
        }
    }

    end = clock();
    end_time = time(NULL);
    cpu_time = (double)(end - start) / CLOCKS_PER_SEC;
    elapsed_time = difftime(end_time, start_time);

    printf("Execution time of Loop Reordering (CPU time): %f seconds\n", cpu_time);
    printf("elapsed_time: ",elapsed_time);
    // Free allocated memory
    for (i = 0; i < m; i++) {
        free(A[i]);
        free(C[i]);
    }
    for (i = 0; i < n; i++) {
        free(B[i]);
    }
    free(A);
    free(B);
    free(C);
}
void loop_i_j_k(void) {
    time_t start_time, end_time;
    clock_t start, end;
    double elapsed_time, cpu_time;
    int m = 500;  // Rows in matrix A and C
    int n = 500;  // Columns in matrix A and rows in matrix B
    int p = 500;  // Columns in matrix B and C
    double** A, ** B, ** C;
    int i, j, k;
    double f;

    // Allocate memory for matrices A, B, and C
    A = (double**)malloc(m * sizeof(double*));
    B = (double**)malloc(n * sizeof(double*));
    C = (double**)malloc(m * sizeof(double*));
    for (i = 0; i < m; i++) {
        A[i] = (double*)malloc(n * sizeof(double));
        C[i] = (double*)malloc(p * sizeof(double));
    }
    for (i = 0; i < n; i++) {
        B[i] = (double*)malloc(p * sizeof(double));
    }

    start_time = time(NULL);
    start = clock(); // Record the start time

    // Initialize matrices A and B with double-precision values
    for (i = 0; i < m; i++) {
        for (j = 0; j < n; j++) {
            A[i][j] = 1.1;
        }
    }

    for (i = 0; i < n; i++) {
        for (j = 0; j < p; j++) {
            B[i][j] = 1.1;
        }
    }
    
    

    // Matrix multiplication
    for (i = 0; i < m; i++) {
        for (j = 0; j < p; j++) {
            f = 0;
            for (k = 0; k < n; k++) {
                f += A[i][k] * B[k][j];
            }
            C[i][j] = f;
        }
    }

    end = clock();
    end_time = time(NULL);
    cpu_time = (double)(end - start) / CLOCKS_PER_SEC;
    elapsed_time = difftime(end_time, start_time);
    printf("Execution time of Loop-i-j-k (CPU time): %f seconds\n", cpu_time);

    // Free allocated memory
    for (i = 0; i < m; i++) {
        free(A[i]);
        free(C[i]);
    }
    for (i = 0; i < n; i++) {
        free(B[i]);
    }
    free(A);
    free(B);
    free(C);
}


void matrix_column(void) {
    time_t start_time, end_time;
    clock_t start, end;
    double elapsed_time, cpu_time;
    int m = 500;  // Rows in matrix A and C
    int n = 500;  // Columns in matrix A and rows in matrix B
    int p = 500;  // Columns in matrix B and C
    double** A, ** B, ** C;
    int i, j, k;

    // Allocate memory for matrices A, B, and C
    A = (double**)malloc(m * sizeof(double*));
    B = (double**)malloc(n * sizeof(double*));
    C = (double**)malloc(m * sizeof(double*));
    for (i = 0; i < m; i++) {
        A[i] = (double*)malloc(n * sizeof(double));
        C[i] = (double*)malloc(p * sizeof(double));
    }
    for (i = 0; i < n; i++) {
        B[i] = (double*)malloc(p * sizeof(double));
    }

    start_time = time(NULL);
    start = clock(); // Record the start time

    // Initialize matrices A and B with double-precision values
    for (i = 0; i < m; i++) {
        for (j = 0; j < n; j++) {
            A[i][j] = 1.1;
        }
    }

    for (i = 0; i < n; i++) {
        for (j = 0; j < p; j++) {
            B[i][j] = 1.1;
        }
    }


    // Matrix multiplication with (Matrix–column)
    for (j = 0; j < p; j++) {
        for (i = 0; i < m; i++) {
            C[i][j] = 0;
            for (k = 0; k < n; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    end = clock();
    end_time = time(NULL);
    cpu_time = (double)(end - start) / CLOCKS_PER_SEC;
    elapsed_time = difftime(end_time, start_time);

    printf("Execution time of Matrix-Column (CPU time): %f seconds\n", cpu_time);

    // Free allocated memory
    for (i = 0; i < m; i++) {
        free(A[i]);
        free(C[i]);
    }
    for (i = 0; i < n; i++) {
        free(B[i]);
    }
    free(A);
    free(B);
    free(C);
}
void matrix_block_algorithm(void) {
    time_t start_time, end_time;
    clock_t start, end;
    double elapsed_time, cpu_time;
    int m = 500;  // Rows in matrix A and C
    int n = 500;  // Columns in matrix A and rows in matrix B
    int p = 500;  // Columns in matrix B and C
    int block_size = 100;  // Size of the submatrices
    double** A, ** B, ** C;
    int i, j, k, ii, jj, kk;
    double f;

    // Allocate memory for matrices A, B, and C
    A = (double**)malloc(m * sizeof(double*));
    B = (double**)malloc(n * sizeof(double*));
    C = (double**)malloc(m * sizeof(double*));
    for (i = 0; i < m; i++) {
        A[i] = (double*)malloc(n * sizeof(double));
        C[i] = (double*)malloc(p * sizeof(double));
    }
    for (i = 0; i < n; i++) {
        B[i] = (double*)malloc(p * sizeof(double));
    }

    start_time = time(NULL);
    start = clock(); // Record the start time

    // Initialize matrices A and B with double-precision values
    for (i = 0; i < m; i++) {
        for (j = 0; j < n; j++) {
            A[i][j] = 1.1;
        }
    }

    for (i = 0; i < n; i++) {
        for (j = 0; j < p; j++) {
            B[i][j] = 1.1;
        }
    }

    

    // Matrix multiplication with block algorithm without submatrix copy
    for (ii = 0; ii < m; ii += block_size) {
        for (jj = 0; jj < p; jj += block_size) {
            for (kk = 0; kk < n; kk += block_size) {
                for (i = ii; i < ii + block_size; i++) {
                    for (j = jj; j < jj + block_size; j++) {
                        f = 0;
                        for (k = kk; k < kk + block_size; k++) {
                            f += A[i][k] * B[k][j];
                        }
                        C[i][j] += f;
                    }
                }
            }
        }
    }

    end = clock();
    end_time = time(NULL);
    cpu_time = (double)(end - start) / CLOCKS_PER_SEC;
    elapsed_time = difftime(end_time, start_time);

    printf("Execution time of Block Algorithm (without submatrix copy) (CPU time): %f seconds\n", cpu_time);

    // Free allocated memory
    for (i = 0; i < m; i++) {
        free(A[i]);
        free(C[i]);
    }
    for (i = 0; i < n; i++) {
        free(B[i]);
    }
    free(A);
    free(B);
    free(C);
}

void matrix_block_algorithm_copy(void) {
    time_t start_time, end_time;
    clock_t start, end;
    double elapsed_time, cpu_time;
    int m = 500;  // Rows in matrix A and C
    int n = 500;  // Columns in matrix A and rows in matrix B
    int p = 500;  // Columns in matrix B and C
    int block_size = 10;  // Size of the submatrices
    double** A, ** B, ** C, ** A_sub, ** B_sub;
    int i, j, k, ii, jj, kk;
    double f;

    // Allocate memory for matrices A, B, C, and submatrices A_sub and B_sub
    A = (double**)malloc(m * sizeof(double*));
    B = (double**)malloc(n * sizeof(double*));
    C = (double**)malloc(m * sizeof(double*));
    A_sub = (double**)malloc(block_size * sizeof(double*));
    B_sub = (double**)malloc(block_size * sizeof(double*));

    for (i = 0; i < m; i++) {
        A[i] = (double*)malloc(n * sizeof(double));
        C[i] = (double*)malloc(p * sizeof(double));
    }
    for (i = 0; i < n; i++) {
        B[i] = (double*)malloc(p * sizeof(double));
    }
    for (i = 0; i < block_size; i++) {
        A_sub[i] = (double*)malloc(block_size * sizeof(double));
        B_sub[i] = (double*)malloc(block_size * sizeof(double));
    }

    start_time = time(NULL);
    start = clock(); // Record the start time
    // Initialize matrices A and B with double-precision values
    for (i = 0; i < m; i++) {
        for (j = 0; j < n; j++) {
            A[i][j] = 1.1;
        }
    }

    for (i = 0; i < n; i++) {
        for (j = 0; j < p; j++) {
            B[i][j] = 1.1;
        }
    }

    

    // Matrix multiplication with block algorithm and submatrices copy
    for (ii = 0; ii < m; ii += block_size) {
        for (jj = 0; jj < p; jj += block_size) {
            for (kk = 0; kk < n; kk += block_size) {
                // Copy submatrix A into A_sub
                for (i = 0; i < block_size; i++) {
                    for (k = 0; k < block_size; k++) {
                        A_sub[i][k] = A[ii + i][kk + k];
                    }
                }

                // Copy submatrix B into B_sub
                for (k = 0; k < block_size; k++) {
                    for (j = 0; j < block_size; j++) {
                        B_sub[k][j] = B[kk + k][jj + j];
                    }
                }

                // Multiply submatrices and accumulate the result
                for (i = 0; i < block_size; i++) {
                    for (j = 0; j < block_size; j++) {
                        f = 0;
                        for (k = 0; k < block_size; k++) {
                            f += A_sub[i][k] * B_sub[k][j];
                        }
                        C[ii + i][jj + j] += f;
                    }
                }
            }
        }
    }

    end = clock();
    end_time = time(NULL);
    cpu_time = (double)(end - start) / CLOCKS_PER_SEC;
    elapsed_time = difftime(end_time, start_time);

    printf("Execution time of Block Algorithm (with submatrices copy) (CPU time): %f seconds\n", cpu_time);

    // Free allocated memory
    for (i = 0; i < m; i++) {
        free(A[i]);
        free(C[i]);
    }
    for (i = 0; i < n; i++) {
        free(B[i]);
    }
    for (i = 0; i < block_size; i++) {
        free(A_sub[i]);
        free(B_sub[i]);
    }
    free(A);
    free(B);
    free(C);
    free(A_sub);
    free(B_sub);
}

int main() {
    loop_i_j_k();
    loop_reordering();
    matrix_column();
    matrix_block_algorithm();
    matrix_block_algorithm_copy();
    return 0;
}
