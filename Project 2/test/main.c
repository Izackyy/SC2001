#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <time.h>

#define V 7  // Number of vertices in the graph
#define INF 99999 // Define infinity as a large value

// Global variable to keep track of key comparisons
int key_comparisons = 0;

// Function to find the vertex with the minimum distance that is not yet processed
int minDistance(int dist[], int visited[]) {
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
void dijkstra(int graph[V][V], int src) {
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
        int u = minDistance(dist, visited);

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

    // Print the shortest distances from the source vertex
    printf("Vertex \t Distance from Source\n");
    for (int i = 0; i < V; i++) {
        printf("%d \t\t %d\n", i, dist[i]);
    }

    printf("\nTotal number of key comparisons: %d\n", key_comparisons);
}



int main() {
//    // Example adjacency matrix representing the graph
//    int graph[V][V] = { { 0, 4, 0, 0, 0, 0, 0, 8, 0 },
//                        { 4, 0, 8, 0, 0, 0, 0, 11, 0 },
//                        { 0, 8, 0, 7, 0, 4, 0, 0, 2 },
//                        { 0, 0, 7, 0, 9, 14, 0, 0, 0 },
//                        { 0, 0, 0, 9, 0, 10, 0, 0, 0 },
//                        { 0, 0, 4, 14, 10, 0, 2, 0, 0 },
//                        { 0, 0, 0, 0, 0, 2, 0, 1, 6 },
//                        { 8, 11, 0, 0, 0, 0, 1, 0, 7 },
//                        { 0, 0, 2, 0, 0, 0, 6, 7, 0 } };
//

    int E = 6;  // Number of edges

    int graph[V][V];

    generateRandomGraph(graph, E);

    int source = 0;  // Starting vertex
    dijkstra(graph, source);

    return 0;
}

void generateRandomGraph(int graph[][5], int E) {
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

    // Print the adjacency matrix
    printf("Generated Adjacency Matrix:\n");
    for (int i = 0; i < V; i++) {
        for (int j = 0; j < V; j++) {
            if (graph[i][j] == 0) {
                printf("%d ", 0);  // Print INF for no edge
            } else {
                printf("%d ", graph[i][j]);
            }
        }
        printf("\n");
    }
}

