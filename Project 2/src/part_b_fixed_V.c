#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_V 1000
#define MAX_E 1000000
#define STEP 1000

typedef struct {
    int vertex;
    int weight;
} Edge;

typedef struct {
    Edge* edges;
    int edge_count;
} AdjacencyList;

typedef struct {
    int vertex;
    int distance;
} HeapNode;

typedef struct {
    HeapNode* nodes;
    int size;
} MinHeap;

AdjacencyList graph[MAX_V];
int comparisons;

void init_graph(int V) {
    for (int i = 0; i < V; i++) {
        graph[i].edges = malloc(sizeof(Edge) * (MAX_E / V));  // Allocating space for edges
        graph[i].edge_count = 0;
    }
}

void add_edge(int u, int v, int weight) {
    graph[u].edges[graph[u].edge_count++] = (Edge){v, weight};
    graph[v].edges[graph[v].edge_count++] = (Edge){u, weight};  // For undirected graph
}

MinHeap* create_min_heap(int capacity) {
    MinHeap* minHeap = malloc(sizeof(MinHeap));
    minHeap->nodes = malloc(sizeof(HeapNode) * capacity);
    minHeap->size = 0;
    return minHeap;
}

void min_heap_insert(MinHeap* minHeap, int vertex, int distance) {
    minHeap->nodes[minHeap->size++] = (HeapNode){vertex, distance};
}

HeapNode extract_min(MinHeap* minHeap) {
    int minIndex = 0;
    for (int i = 1; i < minHeap->size; i++) {
        if (minHeap->nodes[i].distance < minHeap->nodes[minIndex].distance) {
            minIndex = i;
        }
    }
    HeapNode minNode = minHeap->nodes[minIndex];
    minHeap->nodes[minIndex] = minHeap->nodes[--minHeap->size];  // Remove min
    return minNode;
}

int is_empty(MinHeap* minHeap) {
    return minHeap->size == 0;
}

void dijkstra(int V, int start) {
    int* distances = malloc(sizeof(int) * V);
    for (int i = 0; i < V; i++) {
        distances[i] = INT_MAX;
    }
    distances[start] = 0;

    MinHeap* minHeap = create_min_heap(V);
    min_heap_insert(minHeap, start, 0);

    while (!is_empty(minHeap)) {
        HeapNode minNode = extract_min(minHeap);

        for (int i = 0; i < graph[minNode.vertex].edge_count; i++) {
            comparisons++;
            Edge edge = graph[minNode.vertex].edges[i];
            int distance = minNode.distance + edge.weight;

            if (distance < distances[edge.vertex]) {
                distances[edge.vertex] = distance;
                min_heap_insert(minHeap, edge.vertex, distance);
            }
        }
    }

    free(distances);
    free(minHeap->nodes);
    free(minHeap);
}

void write_results_to_csv(const char* filename, int V, int E, int comparisons) {
    FILE* file = fopen(filename, "a");
    if (file != NULL) {
        fprintf(file, "%d,%d,%d\n", E, V, comparisons);
        fclose(file);
    }
}

void generate_graph(int V, int E) {
    init_graph(V);
    int edges_added = 0;
    int max_possible_edges = V * (V - 1) / 2;

    if (E > max_possible_edges) {
        printf("E exceeds the maximum number of possible edges for the graph.\n");
        return;
    }

    while (edges_added < E) {
        int u = rand() % V;
        int v = rand() % V;
        int weight = (rand() % 10) + 1;

        // Avoid self-loops and duplicate edges
        if (u != v) {
            int duplicate = 0;
            for (int i = 0; i < graph[u].edge_count; i++) {
                if (graph[u].edges[i].vertex == v) {
                    duplicate = 1;
                    break;
                }
            }
            if (!duplicate) {
                add_edge(u, v, weight);
                edges_added++;
            }
        }
    }
}

int main() {
    int V = 1000;  // Fixed number of vertices

    // Create CSV file and write headers
    FILE* file = fopen("dijkstra_results.csv", "w");
    fprintf(file, "E,V,comparisons\n");
    fclose(file);

    for (int E = 1000; E <= MAX_E; E += STEP) {
        comparisons = 0;
        generate_graph(V, E);
        dijkstra(V, 0);  // Start from vertex 0
        write_results_to_csv("dijkstra_results.csv", V, E, comparisons);
    }

    // Free graph memory
    for (int i = 0; i < V; i++) {
        free(graph[i].edges);
    }

    return 0;
}
