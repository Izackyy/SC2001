#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <time.h>

#define INF 99999  // Define infinity as a large value

// Global variable to keep track of key comparisons
int key_comparisons = 0;

// Function to find the vertex with the minimum distance that is not yet processed
int minDistance(int dist[], int visited[], int V) {
    int min = INF, min_index;

    for (int v = 0; v < V; v++) {
        key_comparisons++;  // Count this comparison
        if (visited[v] == 0 && dist[v] <= min) {
            min = dist[v];
            min_index = v;
        }
    }

    return min_index;
}

// Dijkstra's Algorithm using adjacency matrix and array-based priority queue
void dijkstra(int **graph, int V, int src) {
    int dist[V];  // The output array. dist[i] will hold the shortest distance from src to i.
    int visited[V];  // visited[i] will be true if vertex i is included in the shortest path tree

    // Initialize distances as infinity and visited[] as false
    for (int i = 0; i < V; i++) {
        dist[i] = INF;
        visited[i] = 0;
    }

    // Distance of source vertex from itself is always 0
    dist[src] = 0;

    // Find the shortest path for all vertices
    for (int count = 0; count < V - 1; count++) {
        // Pick the minimum distance vertex from the set of vertices not yet processed
        int u = minDistance(dist, visited, V);

        // Mark the picked vertex as processed
        visited[u] = 1;

        // Update dist[] of the adjacent vertices of the picked vertex
        for (int v = 0; v < V; v++) {
            key_comparisons++;
            // Update dist[v] only if it's not visited, there is an edge from u to v,
            // and the total weight of the path from src to v through u is smaller than the current value of dist[v]
            if (!visited[v] && graph[u][v] && dist[u] != INF && dist[u] + graph[u][v] < dist[v]) {
                dist[v] = dist[u] + graph[u][v];
            }
        }
    }
}

// Function to generate a random graph with a fixed number of edges
void generateRandomGraph(int **graph, int V, int E) {
    // Initialize the adjacency matrix to all 0 (no edges)
    for (int i = 0; i < V; i++) {
        for (int j = 0; j < V; j++) {
            graph[i][j] = 0;
        }
    }

    int edges_added = 0;
    srand(time(0));  // Seed for random number generation

    // Keep adding edges until we have exactly E edges
    while (edges_added < E) {
        int u = rand() % V;  // Random vertex u
        int v = rand() % V;  // Random vertex v

        // Ensure that we don't add an edge between the same vertex and that the edge doesn't already exist
        if (u != v && graph[u][v] == 0) {
            int weight = (rand() % 10) + 1;  // Generate a random weight between 1 and 10
            graph[u][v] = weight;
            graph[v][u] = weight;  // For undirected graphs, mirror the edge

            edges_added++;
        }
    }


}

// Function to write results to CSV
void writeToCSV(FILE *fp, int E, int comparisons) {
    fprintf(fp, "%d,%d\n", E, comparisons);
}

int main() {
    int V = 1000;  // Fixed number of vertices
    int startE = 10000;  // Starting number of edges
    int endE = 450000;  // End number of edges
    int step = 10000;  // Step size for edges

    // Open the CSV file to save the results
    FILE *fp = fopen("part_a_varyE.csv", "w");
    if (fp == NULL) {
        printf("Unable to create file.\n");
        return 1;
    }

    // Write the header for the CSV file
    fprintf(fp, "Edges,Comparisons\n");

    for (int E = startE; E <= endE; E += step) {
        // Dynamically allocate a 2D array for the adjacency matrix
        int **graph = (int **)malloc(V * sizeof(int *));
        for (int i = 0; i < V; i++) {
            graph[i] = (int *)malloc(V * sizeof(int));
        }


        // Generate a random graph with V vertices and E edges
        generateRandomGraph(graph, V, E);

        printf("Graph generated\n");

        // Reset key comparisons counter
        key_comparisons = 0;

        // Run Dijkstra's algorithm from the source vertex 0
        dijkstra(graph, V, 0);

        // Write the number of comparisons and edges to the CSV file
        writeToCSV(fp, E, key_comparisons);

        // Free the dynamically allocated memory
        for (int i = 0; i < V; i++) {
            free(graph[i]);
        }
        free(graph);

        // Display the progress
        printf("Completed for E = %d\n", E);
    }

    // Close the CSV file
    fclose(fp);

    printf("Results have been saved to dijkstra_results.csv\n");

    return 0;
}
