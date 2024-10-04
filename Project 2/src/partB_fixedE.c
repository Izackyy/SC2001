#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define INF 9999999  // Define a large value to represent infinity

// A structure to represent a min-heap node
struct MinHeapNode {
    int v;
    int dist;
};

// A structure to represent a min-heap
struct MinHeap {
    int size;
    int capacity;
    int *pos;  // To track positions of nodes in the heap
    struct MinHeapNode **array;
};

// A structure to represent an edge in the adjacency list
struct Edge {
    int dest;
    int weight;
    struct Edge *next;
};

// A structure to represent a graph using an adjacency list
struct AdjList {
    struct Edge *head;
};

struct Graph {
    int V;  // Number of vertices
    struct AdjList *array;
};

// Function to create a new min-heap node
struct MinHeapNode* newMinHeapNode(int v, int dist) {
    struct MinHeapNode* minHeapNode = (struct MinHeapNode*) malloc(sizeof(struct MinHeapNode));
    minHeapNode->v = v;
    minHeapNode->dist = dist;
    return minHeapNode;
}

// Function to create a min-heap
struct MinHeap* createMinHeap(int capacity) {
    struct MinHeap* minHeap = (struct MinHeap*) malloc(sizeof(struct MinHeap));
    minHeap->pos = (int *) malloc(capacity * sizeof(int));
    minHeap->size = 0;
    minHeap->capacity = capacity;
    minHeap->array = (struct MinHeapNode**) malloc(capacity * sizeof(struct MinHeapNode*));
    return minHeap;
}

// Function to create a graph with V vertices
struct Graph* createGraph(int V) {
    struct Graph* graph = (struct Graph*) malloc(sizeof(struct Graph));
    graph->V = V;
    graph->array = (struct AdjList*) malloc(V * sizeof(struct AdjList));
    for (int i = 0; i < V; ++i) {
        graph->array[i].head = NULL;
    }
    return graph;
}

// Function to add an edge to the graph
void addEdge(struct Graph* graph, int src, int dest, int weight) {
    struct Edge* newEdge = (struct Edge*) malloc(sizeof(struct Edge));
    newEdge->dest = dest;
    newEdge->weight = weight;
    newEdge->next = graph->array[src].head;
    graph->array[src].head = newEdge;
}

// Function to swap two nodes of the heap
void swapMinHeapNode(struct MinHeapNode** a, struct MinHeapNode** b) {
    struct MinHeapNode* temp = *a;
    *a = *b;
    *b = temp;
}

// Heapify at index i
void minHeapify(struct MinHeap* minHeap, int idx, int* comparisonCount) {
    int smallest, left, right;
    smallest = idx;
    left = 2 * idx + 1;
    right = 2 * idx + 2;

    (*comparisonCount)++;
    if (left < minHeap->size && minHeap->array[left]->dist < minHeap->array[smallest]->dist) {
        smallest = left;
    }

    (*comparisonCount)++;
    if (right < minHeap->size && minHeap->array[right]->dist < minHeap->array[smallest]->dist) {
        smallest = right;
    }

    if (smallest != idx) {
        struct MinHeapNode* smallestNode = minHeap->array[smallest];
        struct MinHeapNode* idxNode = minHeap->array[idx];

        // Swap positions
        minHeap->pos[smallestNode->v] = idx;
        minHeap->pos[idxNode->v] = smallest;

        // Swap nodes
        swapMinHeapNode(&minHeap->array[smallest], &minHeap->array[idx]);

        // Recursively heapify the affected sub-tree
        minHeapify(minHeap, smallest, comparisonCount);
    }
}

// Extract the node with the minimum distance
struct MinHeapNode* extractMin(struct MinHeap* minHeap, int* comparisonCount) {
    if (minHeap->size == 0) {
        return NULL;
    }

    // Store the root node
    struct MinHeapNode* root = minHeap->array[0];

    // Replace root with the last node
    struct MinHeapNode* lastNode = minHeap->array[minHeap->size - 1];
    minHeap->array[0] = lastNode;

    // Update positions
    minHeap->pos[root->v] = minHeap->size - 1;
    minHeap->pos[lastNode->v] = 0;

    // Reduce heap size and heapify the root
    minHeap->size--;
    minHeapify(minHeap, 0, comparisonCount);

    return root;
}

// Function to decrease distance value of a vertex
void decreaseKey(struct MinHeap* minHeap, int v, int dist, int* comparisonCount) {
    int i = minHeap->pos[v];
    minHeap->array[i]->dist = dist;

    // Move up while min-heap property is violated
    while (i && minHeap->array[i]->dist < minHeap->array[(i - 1) / 2]->dist) {
        (*comparisonCount)++;
        // Swap node with its parent
        minHeap->pos[minHeap->array[i]->v] = (i - 1) / 2;
        minHeap->pos[minHeap->array[(i - 1) / 2]->v] = i;
        swapMinHeapNode(&minHeap->array[i], &minHeap->array[(i - 1) / 2]);

        i = (i - 1) / 2;
    }
}

// Function to check if a given vertex is in min-heap or not
int isInMinHeap(struct MinHeap* minHeap, int v) {
    if (minHeap->pos[v] < minHeap->size) {
        return 1;
    }
    return 0;
}

// Function to implement Dijkstra's algorithm with comparison counting
void dijkstra(struct Graph* graph, int src, int* comparisonCount) {
    int V = graph->V;  // Number of vertices
    int dist[V];       // Output array. dist[i] will hold the shortest distance from src to i

    // Create a min-heap and initialize it
    struct MinHeap* minHeap = createMinHeap(V);

    // Initialize distances
    for (int v = 0; v < V; ++v) {
        dist[v] = INF;
        minHeap->array[v] = newMinHeapNode(v, dist[v]);
        minHeap->pos[v] = v;
    }

    // Set distance of the source vertex
    dist[src] = 0;
    decreaseKey(minHeap, src, dist[src], comparisonCount);

    minHeap->size = V;

    // Loop until the min-heap is empty
    while (minHeap->size > 0) {
        // Extract the vertex with the minimum distance
        struct MinHeapNode* minHeapNode = extractMin(minHeap, comparisonCount);
        int u = minHeapNode->v;

        // Traverse all adjacent vertices of the extracted vertex
        struct Edge* pCrawl = graph->array[u].head;
        while (pCrawl != NULL) {
            int v = pCrawl->dest;

            // Relax the edge
            (*comparisonCount)++;
            if (isInMinHeap(minHeap, v) && dist[u] != INF && pCrawl->weight + dist[u] < dist[v]) {
                dist[v] = dist[u] + pCrawl->weight;
                decreaseKey(minHeap, v, dist[v], comparisonCount);
            }
            pCrawl = pCrawl->next;
        }
    }
}

// Function to generate a random graph with E edges
void generateRandomGraph(struct Graph* graph, int E) {
    srand(time(0));
    int V = graph->V;

    for (int i = 0; i < E; ++i) {
        int u = rand() % V;
        int v = rand() % V;
        int weight = (rand() % 10) + 1;

        if (u != v) {
            addEdge(graph, u, v, weight);
        }
    }
}

void saveToCSV(int ccount, int V) {

    char filename[] = "part_b_fixedE.csv";

    // Open the file for writing ("w" for write mode)
    FILE *file = fopen(filename, "a");

    // Write the integer, float, and string data for each row
    fprintf(file, "%d,%d\n", V, ccount);

    // Close the file after writing
    fclose(file);

}

int main() {
    int comparisonCount;
    int E = 500000;

    for (int V = 1000; V<E; V+=1000) {

        // Create a graph
        struct Graph* graph = createGraph(V);
        generateRandomGraph(graph, E);

        // Reset the comparison count
        comparisonCount = 0;

        // Run Dijkstra's algorithm
        dijkstra(graph, 0, &comparisonCount);

        //Save data to CSV
        saveToCSV(comparisonCount, V);

        printf("%d:%d\n",V,comparisonCount);

    }

    return 0;
}
