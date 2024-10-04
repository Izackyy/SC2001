#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <time.h>

#define V 7  // Number of vertices in the graph
#define INF 99999 // Define infinity as a large value

// Structure to represent a min heap node
typedef struct MinHeapNode {
    int v;       // Vertex number
    int dist;    // Distance value
} MinHeapNode;

// Structure to represent a min heap
typedef struct MinHeap {
    int size;      // Number of heap nodes
    int capacity;  // Capacity of the heap
    int *pos;      // Position of vertices in heap
    MinHeapNode **array; // Heap node array
} MinHeap;

// Function to create a new min heap node
MinHeapNode* newMinHeapNode(int v, int dist) {
    MinHeapNode* minHeapNode = (MinHeapNode*) malloc(sizeof(MinHeapNode));
    minHeapNode->v = v;
    minHeapNode->dist = dist;
    return minHeapNode;
}

// Function to create a min heap
MinHeap* createMinHeap(int capacity) {
    MinHeap* minHeap = (MinHeap*) malloc(sizeof(MinHeap));
    minHeap->pos = (int*) malloc(capacity * sizeof(int));
    minHeap->size = 0;
    minHeap->capacity = capacity;
    minHeap->array = (MinHeapNode**) malloc(capacity * sizeof(MinHeapNode*));
    return minHeap;
}

// Function to swap two nodes in the min heap
void swapMinHeapNode(MinHeapNode** a, MinHeapNode** b) {
    MinHeapNode* t = *a;
    *a = *b;
    *b = t;
}

// Function to heapify at a given index
void minHeapify(MinHeap* minHeap, int idx) {
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    if (left < minHeap->size && minHeap->array[left]->dist < minHeap->array[smallest]->dist) {
        smallest = left;
    }

    if (right < minHeap->size && minHeap->array[right]->dist < minHeap->array[smallest]->dist) {
        smallest = right;
    }

    if (smallest != idx) {
        MinHeapNode* smallestNode = minHeap->array[smallest];
        MinHeapNode* idxNode = minHeap->array[idx];

        // Swap the positions
        minHeap->pos[smallestNode->v] = idx;
        minHeap->pos[idxNode->v] = smallest;

        // Swap nodes
        swapMinHeapNode(&minHeap->array[smallest], &minHeap->array[idx]);

        minHeapify(minHeap, smallest);
    }
}

// Function to extract the minimum node from the heap
MinHeapNode* extractMin(MinHeap* minHeap) {
    if (minHeap->size == 0) {
        return NULL;
    }

    // Store the root node
    MinHeapNode* root = minHeap->array[0];

    // Replace the root with the last node
    MinHeapNode* lastNode = minHeap->array[minHeap->size - 1];
    minHeap->array[0] = lastNode;

    // Update position of last node
    minHeap->pos[root->v] = minHeap->size - 1;
    minHeap->pos[lastNode->v] = 0;

    // Reduce heap size and heapify root
    minHeap->size--;
    minHeapify(minHeap, 0);

    return root;
}

// Function to decrease the distance value of a vertex v
void decreaseKey(MinHeap* minHeap, int v, int dist) {
    int i = minHeap->pos[v];
    minHeap->array[i]->dist = dist;

    // Fix the min heap property if violated
    while (i && minHeap->array[i]->dist < minHeap->array[(i - 1) / 2]->dist) {
        minHeap->pos[minHeap->array[i]->v] = (i - 1) / 2;
        minHeap->pos[minHeap->array[(i - 1) / 2]->v] = i;
        swapMinHeapNode(&minHeap->array[i], &minHeap->array[(i - 1) / 2]);

        i = (i - 1) / 2;
    }
}

// Function to check if a vertex is in the min heap
int isInMinHeap(MinHeap *minHeap, int v) {
    return minHeap->pos[v] < minHeap->size;
}

// Dijkstra's algorithm using a min-heap-based priority queue
void dijkstra(int graph[V][V], int src) {
    int dist[V];  // The output array. dist[i] will hold the shortest distance from src to i.

    // Min heap to store vertices not yet processed
    MinHeap* minHeap = createMinHeap(V);

    // Initialize the heap and distances
    for (int v = 0; v < V; ++v) {
        dist[v] = INF;
        minHeap->array[v] = newMinHeapNode(v, dist[v]);
        minHeap->pos[v] = v;
    }

    // Set the distance to the source vertex to 0 and insert into the heap
    minHeap->array[src] = newMinHeapNode(src, dist[src]);
    minHeap->pos[src] = src;
    dist[src] = 0;
    decreaseKey(minHeap, src, dist[src]);

    minHeap->size = V;

    while (minHeap->size != 0) {
        // Extract the vertex with the minimum distance
        MinHeapNode* minHeapNode = extractMin(minHeap);
        int u = minHeapNode->v;

        // Update distance of the adjacent vertices
        for (int v = 0; v < V; ++v) {
            if (graph[u][v] && isInMinHeap(minHeap, v) && dist[u] != INF && dist[u] + graph[u][v] < dist[v]) {
                dist[v] = dist[u] + graph[u][v];
                decreaseKey(minHeap, v, dist[v]);
            }
        }
    }

    // Print the distances
    printf("Vertex   Distance from Source\n");
    for (int i = 0; i < V; ++i) {
        printf("%d \t\t %d\n", i, dist[i]);
    }
}

// Function to generate a random graph (same as the original)
void generateRandomGraph(int graph[][V], int E) {
    for (int i = 0; i < V; i++) {
        for (int j = 0; j < V; j++) {
            graph[i][j] = 0;
        }
    }

    int edges_added = 0;
    srand(time(0));

    while (edges_added < E) {
        int u = rand() % V;
        int v = rand() % V;

        if (u != v && graph[u][v] == 0) {
            int weight = (rand() % 10) + 1;
            graph[u][v] = weight;
            graph[v][u] = weight;
            edges_added++;
        }
    }

    printf("Generated Adjacency Matrix:\n");
    for (int i = 0; i < V; i++) {
        for (int j = 0; j < V; j++) {
            if (graph[i][j] == 0) {
                printf("%d ", 0);
            } else {
                printf("%d ", graph[i][j]);
            }
        }
        printf("\n");
    }
}

int main() {
    int E = 6;  // Number of edges
    int graph[V][V];

    generateRandomGraph(graph, E);
    int source = 0;
    dijkstra(graph, source);

    return 0;
}
