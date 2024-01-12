#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define INF 99999
#define PATH_NONE -1

void printMatrix(int n, int *matrix) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (matrix[i * n + j] == INF)
                printf("%5s", "INF");
            else
                printf("%5d", matrix[i * n + j]);
        }
        printf("\n");
    }
}

void printPath(int *path, int n, int start, int end) {
    if (start == end) {
        printf("%d", start);
        return;
    }
    if (path[start * n + end] == PATH_NONE) {
        printf("Path does not exist");
        return;
    }
    printPath(path, n, start, path[start * n + end]);
    printf(" -> %d", end);
}

void floydWarshallParallel(int rank, int size, int n, int *local_dist, int *local_path, int localRowCount, int *rowsPerProcess) {
    int *kthRowDist = (int *)malloc(n * sizeof(int));
    int *kthRowPath = (int *)malloc(n * sizeof(int));

    for (int k = 0; k < n; k++) {
        // Determine the root for broadcasting
        int rowCount = 0;
        int root = 0;
        for (int i = 0; i < size; i++) {
            rowCount += rowsPerProcess[i] / n; // Convert to row count
            if (k < rowCount) {
                root = i;
                break;
            }
        }

        // Broadcast the k-th row from the root
        if (rank == root) {
            int local_k = k - (rowCount - localRowCount);
            for (int j = 0; j < n; j++) {
                kthRowDist[j] = local_dist[local_k * n + j];
                kthRowPath[j] = local_path[local_k * n + j];
            }
        }

        MPI_Bcast(kthRowDist, n, MPI_INT, root, MPI_COMM_WORLD);
        MPI_Bcast(kthRowPath, n, MPI_INT, root, MPI_COMM_WORLD);

        for (int i = 0; i < localRowCount; i++) {
            for (int j = 0; j < n; j++) {
                if (local_dist[i * n + k] + kthRowDist[j] < local_dist[i * n + j]) {
                    local_dist[i * n + j] = local_dist[i * n + k] + kthRowDist[j];
                    local_path[i * n + j] = kthRowPath[k];
                }
            }
        }
    }

    free(kthRowDist);
    free(kthRowPath);
}

int main(int argc, char *argv[]) {
    int rank, size, n;
    int *graph = NULL, *path = NULL;
    int *local_dist, *local_path;
    int *rowsPerProcess, *displacements;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    rowsPerProcess = (int *)malloc(size * sizeof(int));
    displacements = (int *)malloc(size * sizeof(int));
    double start_time, end_time;

    if (rank == 0) {
        printf("Enter the number of vertices: ");
        scanf("%d", &n);

        graph = (int *)malloc(n * n * sizeof(int));
        path = (int *)malloc(n * n * sizeof(int));

        printf("Enter the adjacency matrix (use %d to represent infinity):\n", INF);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                scanf("%d", &graph[i * n + j]);
                if (graph[i * n + j] == 0 && i != j) {
                    graph[i * n + j] = INF;
                }
                path[i * n + j] = (graph[i * n + j] == INF || i == j) ? PATH_NONE : i;
            }
        }

        printf("Initial graph:\n");
        printMatrix(n, graph);

        start_time = MPI_Wtime(); 

        int rowsRemaining = n;
        int sumRows = 0; // Sum of rows assigned so far
        for (int i = 0; i < size; i++) {
            rowsPerProcess[i] = (rowsRemaining / (size - i)) * n; // Convert to the number of elements
            displacements[i] = sumRows * n; // Displacement in terms of elements
            sumRows += rowsRemaining / (size - i);
            rowsRemaining -= rowsRemaining / (size - i);
        }

        int baseRowsPerProcess = n / size;
        int extraRows = n % size;
        int totalElementsHandled = 0;

        for (int i = 0; i < size; i++) {
            int rowsForThisProcess = baseRowsPerProcess + (i < extraRows ? 1 : 0);
            rowsPerProcess[i] = rowsForThisProcess * n; // Elements, not rows
            displacements[i] = totalElementsHandled;
            totalElementsHandled += rowsPerProcess[i];
        }
    }

    // Broadcast n, rowsPerProcess, and displacements
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(rowsPerProcess, size, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(displacements, size, MPI_INT, 0, MPI_COMM_WORLD);

    // Determine the local row count for each process
    int localRowCount = rowsPerProcess[rank] / n; // Local row count for this process
    local_dist = (int *)malloc(localRowCount * n * sizeof(int));
    local_path = (int *)malloc(localRowCount * n * sizeof(int));



    // Scatter the graph and path matrices
    MPI_Scatterv(graph, rowsPerProcess, displacements, MPI_INT, local_dist, localRowCount * n, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatterv(path, rowsPerProcess, displacements, MPI_INT, local_path, localRowCount * n, MPI_INT, 0, MPI_COMM_WORLD);
   
    // Execute the parallel Floyd-Warshall algorithm
    floydWarshallParallel(rank, size, n, local_dist, local_path, localRowCount, rowsPerProcess);

    // Gather the results
    MPI_Gatherv(local_dist, localRowCount * n, MPI_INT, graph, rowsPerProcess, displacements, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Gatherv(local_path, localRowCount * n, MPI_INT, path, rowsPerProcess, displacements, MPI_INT, 0, MPI_COMM_WORLD);
    // Root process prints the results
    if (rank == 0) {
        printf("Shortest distances matrix:\n");
        printMatrix(n, graph);
        //printf("Shortest paths:\n");
        //for (int i = 0; i < n; i++) {
            //for (int j = 0; j < n; j++) {
                //if (i != j && graph[i * n + j] != INF) {
                    //printf("Path from %d to %d (cost %d): ", i, j, graph[i * n + j]);
                    //printPath(path, n, i, j);
                    //printf("\n");
                //}
            //}
        //}
        end_time = MPI_Wtime();  // Stop timing
        printf("Execution Time: %f seconds\n", end_time - start_time);
        free(graph);
        free(path);
    }
    
    free(local_dist);
    free(local_path);
    free(rowsPerProcess);
    free(displacements);
    
    MPI_Finalize();
    return 0;
}
