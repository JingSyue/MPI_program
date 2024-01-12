#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define INF 99999

void printSolution(int n, int **dist);
void printPath(int **path, int i, int j);

void floydWarshall(int n, int **graph) {
    int **dist, **path;
    int i, j, k;

    // Allocate memory for the distance and path matrices
    dist = (int **)malloc(n * sizeof(int *));
    path = (int **)malloc(n * sizeof(int *));
    for (i = 0; i < n; i++) {
        dist[i] = (int *)malloc(n * sizeof(int));
        path[i] = (int *)malloc(n * sizeof(int));
        for (j = 0; j < n; j++) {
            dist[i][j] = graph[i][j]; // Initialize the distance same as graph

            if (i == j || graph[i][j] == INF)
                path[i][j] = -1;
            else
                path[i][j] = i;
        }
    }

    // Implementing the Floyd-Warshall algorithm
    for (k = 0; k < n; k++) {
        for (i = 0; i < n; i++) {
            for (j = 0; j < n; j++) {
                if (dist[i][k] + dist[k][j] < dist[i][j]) {
                    dist[i][j] = dist[i][k] + dist[k][j];
                    path[i][j] = path[k][j];
                }
            }
        }
    }

    // Print the shortest distance matrix
    printSolution(n, dist);

    // Print the shortest paths and their costs
    //printf("\nThe shortest paths from each vertex to each other vertex along with their costs:\n");
    //for (i = 0; i < n; i++) {
        //for (j = 0; j < n; j++) {
            //if (i != j && dist[i][j] != INF) {
                //printf("Shortest path from %d to %d with cost %d: ", i, j, dist[i][j]);
                //printPath(path, i, j);
                //printf("%d\n", j); // Destination vertex
            //}
        //}
    //}

    // Free the dynamically allocated memory
    for (i = 0; i < n; i++) {
        free(dist[i]);
        free(path[i]);
    }
    free(dist);
    free(path);
}

void printSolution(int n, int **dist) {
    printf("The following matrix shows the shortest distances between every pair of vertices:\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (dist[i][j] == INF)
                printf("%7s", "INF");
            else
                printf("%7d", dist[i][j]);
        }
        printf("\n");
    }
}

// Function to print the shortest path from i to j
void printPath(int **path, int i, int j) {
    if (path[i][j] == i)
        return;

    printPath(path, i, path[i][j]);
    printf("%d -> ", path[i][j]);
}

int main() {
    int n, **graph;
    clock_t start, end;
    double cpu_time_used;

    printf("Enter the number of vertices: ");
    scanf("%d", &n);

    // Allocate memory for graph
    graph = (int **)malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++) {
        graph[i] = (int *)malloc(n * sizeof(int));
    }

    printf("Enter the adjacency matrix (use %d to represent infinity):\n", INF);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            scanf("%d", &graph[i][j]);
            if (graph[i][j] == 0 && i != j) {
                graph[i][j] = INF; // No path
            }
        }
    }

    start = clock(); // Start time
    floydWarshall(n, graph);
    end = clock(); // End time

    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Execution time: %f seconds\n", cpu_time_used);

    // Free the dynamically allocated memory
    for (int i = 0; i < n; i++) {
        free(graph[i]);
    }
    free(graph);

    return 0;
}
