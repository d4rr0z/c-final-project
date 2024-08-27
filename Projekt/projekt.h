#pragma once

typedef struct {
    int** adjMatrix;
    int numNodes;
} Graph;

typedef struct {
    int visited;
    int distance;
    int previous;
    int stack;
} Node;

Graph* createGraph(int numNodes);
void destroyGraph(Graph* graph);
void printGraph(Graph* graph);
int addEdge(Graph* graph, int from, int to, int value);
int removeEdge(Graph* graph, int from, int to);
int addNode(Graph* graph);
int removeNode(Graph* graph, int node);
int saveGraph(Graph* graph, const char* filename);
Graph* loadGraph(const char* filename);
void shortestPath(Graph* graph, int startNode);
void longestPath(Graph* graph, int startNode);
int hasCycleDFS(Graph* graph, Node* nodes, int curNode);
int hasCycle(Graph* graph);