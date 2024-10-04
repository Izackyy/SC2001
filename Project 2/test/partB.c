#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define V 10

//create minimizing heap based on lect slides
//implement dijkstra with keyComparisons = 0 INSIDE to reset per loop
//implement saveToCSV

// ListNode structure to represent each node in the adjacency list
typedef struct _listnode {
    int id;                   // The destination vertex
    int weight;               // The weight of the edge
    struct _listnode *next;   // Pointer to the next node in the adjacency list
} ListNode;

// Graph structure to represent the entire graph using an array of adjacency lists
typedef struct _graph {
    int vSize;                // Number of vertices
    int eSize;                // Number of edges
    ListNode **adjL;          // Array of adjacency lists
} Graph;

// Function to create a new ListNode for the adjacency list
ListNode* createListNode(int id, int weight) {
    ListNode* newNode = (ListNode*)malloc(sizeof(ListNode));
    newNode->id = id;
    newNode->weight = weight;
    newNode->next = NULL;
    return newNode;
}

// Function to create a new graph with V vertices
Graph* createGraph(int vSize) {
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    graph->vSize = vSize;
    graph->eSize = 0;

    // Allocate memory for the adjacency list array
    graph->adjL = (ListNode**)malloc(vSize * sizeof(ListNode*));

    // Initialize all adjacency lists to NULL (no edges initially)
    for (int i = 0; i < vSize; i++) {
        graph->adjL[i] = NULL;
    }

    return graph;
}

// Function to add a directed edge to the graph
void addEdge(Graph* graph, int src, int dest, int weight) {
    // Add an edge from src to dest (directed)
    ListNode* newNode = createListNode(dest, weight);
    newNode->next = graph->adjL[src];  // Point the new node to the current head
    graph->adjL[src] = newNode;        // Update the head to point to the new node

    // Increment edge size
    graph->eSize++;
}


// Function to print the adjacency list representation of the graph
void printGraph(Graph* graph) {
    for (int v = 0; v < graph->vSize; v++) {
        ListNode* pCrawl = graph->adjL[v];
        printf("Vertex %d:", v);
        while (pCrawl) {
            printf(" -> (%d, %d)", pCrawl->id, pCrawl->weight);
            pCrawl = pCrawl->next;
        }
        printf("\n");
    }
}

// Function to free the graph memory
void freeGraph(Graph* graph) {
    for (int v = 0; v < graph->vSize; v++) {
        ListNode* temp = graph->adjL[v];
        while (temp) {
            ListNode* toFree = temp;
            temp = temp->next;
            free(toFree);
        }
    }
    free(graph->adjL);
    free(graph);
}

// Function to generate a random directed graph
void generateRandomGraph(Graph* graph, int numEdges) {
    // Seed the random number generator
    srand(time(0));

    int edgesAdded = 0;
    while (edgesAdded < numEdges) {
        // Generate random source and destination vertices
        int src = rand() % graph->vSize;
        int dest = rand() % graph->vSize;

        // Ensure no self-loops (src != dest)
        if (src != dest) {
            // Generate a random weight between 1 and 100
            int weight = rand() % 100 + 1;

            // Add the directed edge to the graph
            addEdge(graph, src, dest, weight);

            // Increment the count of edges added
            edgesAdded++;
        }
    }
}

void swap(int *x, int *y) {
    int temp = *x;
    *x = *y;
    *y = temp;
}

// Function to fix the heap after removing the root
void fixHeap(int heap[], int size, int root) {
    int smallest = root;
    int left = 2 * root + 1;
    int right = 2 * root + 2;

    // Compare root with its children and find the smallest element
    if (left < size && heap[left] < heap[smallest]) {
        smallest = left;
    }
    if (right < size && heap[right] < heap[smallest]) {
        smallest = right;
    }

    // If the smallest is not the root, swap and recursively fix the heap
    if (smallest != root) {
        swap(&heap[root], &heap[smallest]);
        fixHeap(heap, size, smallest);
    }
}

// Function to delete the minimum element (root) from the heap
int deleteMin(int heap[], int *size) {
    int root = heap[0]; // Store the root value (min element)

    // Move the last element to the root
    heap[0] = heap[*size - 1];

    // Reduce heap size
    (*size)--;

    // Restore heap property by fixing the heap
    fixHeap(heap, *size, 0);

    return root; // Return the removed minimum element
}

// Heapifying the array to convert it into a min-heap
void heapify(int heap[], int size, int root) {
    fixHeap(heap, size, root);
}

// Building the min-heap by heapifying from the bottom up
void buildHeap(int heap[], int size) {
    for (int i = size / 2 - 1; i >= 0; i--) {
        fixHeap(heap, size, i);
    }
}

// The heap sort function that uses the min-heap to sort an array
void heapSort(int array[], int n) {
    // Step 1: Build a min-heap from the array
    buildHeap(array, n);

    // Step 2: Extract elements one by one from the heap
    for (int i = n - 1; i >= 0; i--) {
        // Get the minimum element (root of the heap)
        int min = deleteMin(array, &n);

        // Insert the removed min element into the sorted part of the array
        array[i] = min;
    }
}

int main() {
    for(int E = 0; E<V*V; E++){
        // Create the graph
        Graph* graph = createGraph(V);

        // Generate a random graph with V vertices and E edges
        generateRandomGraph(graph, E);

        // Print the graph
        printGraph(graph);

        // Free the graph memory
        freeGraph(graph);
    }
    return 0;
}
