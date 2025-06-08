#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include<string.h>
#define V 9 
// Number of vertices. In your case, it's the number of airports.

// A structure to represent an adjacency list node
struct AdjListNode {
    int dest;
    int weight;
    struct AdjListNode* next;
};

// A structure to represent an adjacency list
struct AdjList {
    struct AdjListNode* head; // pointer to head node of list
};


// A structure to represent a graph. A graph is an array of adjacency lists.
// Size of array will be V (number of vertices in graph)
struct Graph {
    int size;
    struct AdjList* array;
    char** placeNames; // Array of strings to store place names
};

// Function to create a new adjacency list node
struct AdjListNode* newAdjListNode(int dest, int weight) {
    struct AdjListNode* newNode = (struct AdjListNode*) malloc(sizeof(struct AdjListNode));
    newNode->dest = dest;
    newNode->weight = weight;
    newNode->next = NULL;
    return newNode;
}

// A utility function that creates a graph of V vertices
struct Graph* createGraph(int Ve) {
    struct Graph* graph = (struct Graph*) malloc(sizeof(struct Graph));
    graph->size = Ve;
    graph->array = (struct AdjList*) malloc(Ve * sizeof(struct AdjList));
    graph->placeNames = (char*) malloc(Ve * sizeof(char)); // Allocate place names

    for (int i = 0; i < Ve; ++i) {
        graph->array[i].head = NULL;
        graph->placeNames[i] = NULL; // Initialize to NULL
    }

    return graph;
}

// Adds an edge to an undirected graph
void addEdge(struct Graph* graph, int src, int dest, int weight) {
    // Add an edge from src to dest. A new node is added to the adjacency
    // list of src. The node is added at the beginning
    struct AdjListNode* newNode = newAdjListNode(dest, weight);
    newNode->next = graph->array[src].head;
    graph->array[src].head = newNode;

    // Since graph is undirected, add an edge from dest to src also
    newNode = newAdjListNode(src, weight);
    newNode->next = graph->array[dest].head;
    graph->array[dest].head = newNode;
}

// Structure to represent a min heap node
struct MinHeapNode {
    int v;
    int dist;
};

// Structure to represent a min heap
struct MinHeap {
    int size;     // Number of heap nodes present currently
    int capacity; // Capacity of min heap
    int* pos;     // This is needed for decreaseKey()
    struct MinHeapNode** array;
};

// A utility function to create a new Min Heap Node
struct MinHeapNode* newMinHeapNode(int v, int dist) {
    struct MinHeapNode* minHeapNode = (struct MinHeapNode*) malloc(sizeof(struct MinHeapNode));
    minHeapNode->v = v;
    minHeapNode->dist = dist;
    return minHeapNode;
}

// A utility function to create a Min Heap
struct MinHeap* createMinHeap(int capacity) {
    struct MinHeap* minHeap = (struct MinHeap*) malloc(sizeof(struct MinHeap));
    minHeap->pos = (int*)malloc(capacity * sizeof(int));
    minHeap->size = 0;
    minHeap->capacity = capacity;
    minHeap->array = (struct MinHeapNode*) malloc(capacity * sizeof(struct MinHeapNode));
    return minHeap;
}

// A utility function to swap two nodes of the min heap. Needed for min heapify
void swapMinHeapNode(struct MinHeapNode** a, struct MinHeapNode** b) {
    struct MinHeapNode* t = *a;
    *a = *b;
    *b = t;
}

// A standard function to heapify at given idx
// This function also updates position of nodes when they are swapped.
// Position is needed for decreaseKey()
void minHeapify(struct MinHeap* minHeap, int idx) {
    int smallest, left, right;
    smallest = idx;
    left = 2 * idx + 1;
    right = 2 * idx + 2;

    if (left < minHeap->size && minHeap->array[left]->dist < minHeap->array[smallest]->dist)
      smallest = left;

    if (right < minHeap->size && minHeap->array[right]->dist < minHeap->array[smallest]->dist)
      smallest = right;

    if (smallest != idx) {
        // The nodes to be swapped in min heap
        struct MinHeapNode *smallestNode = minHeap->array[smallest];
        struct MinHeapNode *idxNode = minHeap->array[idx];

        // Swap positions
        minHeap->pos[smallestNode->v] = idx;
        minHeap->pos[idxNode->v] = smallest;

        // Swap nodes
        swapMinHeapNode(&minHeap->array[smallest], &minHeap->array[idx]);

        minHeapify(minHeap, smallest);
    }
}

// A utility function to check if the given minHeap is empty or not
int isEmpty(struct MinHeap *minHeap) {
    return minHeap->size == 0;
}

// Standard function to extract minimum node from heap
struct MinHeapNode* extractMin(struct MinHeap* minHeap) {
    if (isEmpty(minHeap))
        return NULL;

    // Store the root node
    struct MinHeapNode* root = minHeap->array[0];

    // Replace root node with last node
    struct MinHeapNode* lastNode = minHeap->array[minHeap->size - 1];
    minHeap->array[0] = lastNode;

    // Update position of last node
    minHeap->pos[root->v] = minHeap->size-1;
    minHeap->pos[lastNode->v] = 0;

    // Reduce heap size and heapify root
    --minHeap->size;
    minHeapify(minHeap, 0);

    return root;
}

// Function to decrease dist value of a given vertex v. This function
// uses pos[] of min heap to get the current index of node in min heap
void decreaseKey(struct MinHeap* minHeap, int v, int dist) {
    // Get the index of v in heap array
    int i = minHeap->pos[v];

    // Get the node and update its dist value
    minHeap->array[i]->dist = dist;

    // Travel up while the complete tree is not heapified.
    // This is a O(Logn) loop

    while (i && minHeap->array[i]->dist < minHeap->array[(i - 1) / 2]->dist) {
        // Swap this node with its parent
        minHeap->pos[minHeap->array[i]->v] = (i-1)/2;
        minHeap->pos[minHeap->array[(i-1)/2]->v] = i;
        swapMinHeapNode(&minHeap->array[i],  &minHeap->array[(i - 1) / 2]);
        
        // move to parent index
        i = (i - 1) / 2;
    }
}

// A utility function to check if a given vertex 'v' is in min heap or not
int isInMinHeap(struct MinHeap *minHeap, int v) {
   if (minHeap->pos[v] < minHeap->size)
     return 1;
   return 0;
}

// The main function that calculates distances of shortest paths from src to all
// vertices. It is a O(ELogV) function
void dijkstra(struct Graph* graph, int src) {
    int Ve = graph->size; // Get the number of vertices in graph
    int dist[Ve];      // dist values used to pick minimum weight edge in cut

    // minHeap represents set E
    struct MinHeap* minHeap = createMinHeap(Ve);

    // Initialize min heap with all vertices. dist value of all vertices 
    for (int v = 0; v < Ve; ++v) {
        dist[v] = INT_MAX;
        minHeap->array[v] = newMinHeapNode(v, dist[v]);
        minHeap->pos[v] = v;
    }

    // Make dist value of src vertex as 0 so that it is extracted first
    minHeap->array[src] = newMinHeapNode(src, dist[src]);
    minHeap->pos[src]   = src;
    dist[src] = 0;
    decreaseKey(minHeap, src, dist[src]);

    // Initially size of min heap is equal to V
    minHeap->size = Ve;

    // In the following loop, min heap contains all nodes
    // whose shortest distance is not yet finalized.
    while (!isEmpty(minHeap)) {
        // Extract the vertex with minimum distance value
        struct MinHeapNode* minHeapNode = extractMin(minHeap);
        int u = minHeapNode->v; // Store the extracted vertex number

        // Traverse through all adjacent vertices of u(the extracted
        // vertex) and update their distance values
        struct AdjListNode* pCrawl = graph->array[u].head;
        while (pCrawl != NULL) {
            int v = pCrawl->dest;

            // If shortest distance to v is not finalized yet, and distance to v
            // through u is less than its previously calculated distance
            if (isInMinHeap(minHeap, v) && dist[u] != INT_MAX && 
                                          pCrawl->weight + dist[u] < dist[v]) {
                dist[v] = dist[u] + pCrawl->weight;

                // update distance value in min heap also
                decreaseKey(minHeap, v, dist[v]);
            }
            pCrawl = pCrawl->next;
        }
    }

    // print the calculated shortest distances
   // print the calculated shortest distances
printf("Distance from the source place %s to other places \n",graph->placeNames[src]);
for (int i = 1; i < Ve; ++i) {
    printf("%s to %s is %d\n", graph->placeNames[src], graph->placeNames[i], dist[i]);
}
}
// Function to free the memory allocated for the graph
void freeGraph(struct Graph* graph) {
    if (graph == NULL)
        return;

    for (int i = 0; i < graph->size; ++i) {
        struct AdjListNode* current = graph->array[i].head;
        while (current != NULL) {
            struct AdjListNode* next = current->next;
            free(current);
            current = next;
        }
        free(graph->placeNames[i]); // Free the place name
    }

    free(graph->placeNames);
    free(graph->array);
    free(graph);
}
int main() 
{
    int Ve, E;
    printf("Enter the number of vertices (places): ");
    scanf("%d", &Ve);
    
    struct Graph* graph = createGraph(Ve);

    printf("Enter each place name:\n");
    char buffer[100]; // Temporary buffer to hold place names
    for (int i = 0; i < Ve; ++i) {
        printf("Place %d: ", i + 1);
        scanf("%s", buffer);
        graph->placeNames[i] = strdup(buffer); // Allocate and copy the name
    }

    printf("Enter the number of edges (connections): ");
    scanf("%d", &E);

    printf("Enter the edges in the format: source place, destination place and distance between them\n");
    for(int i = 0; i < E; i++) {
        int src, dest, weight;
        scanf("%d %d %d", &src, &dest, &weight);
        addEdge(graph, src, dest, weight);
    }

    printf("Dijkstra's Algorithm starting from source vertex (%s):\n", graph->placeNames[0]);
    dijkstra(graph, 0);

    freeGraph(graph); // Remember to free the memory for place names here

    return 0;
}