#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "projekt.h"

Graph* createGraph(int numNodes) {
    Graph* graph = malloc(sizeof(*graph));
    if (graph == NULL) {
        return NULL;
    }
    graph->adjMatrix = calloc(numNodes, sizeof(*(graph->adjMatrix)));
    if (graph->adjMatrix == NULL) {
        free(graph);
        return NULL;
    }
    for (int i = 0; i < numNodes; ++i) {
        graph->adjMatrix[i] = calloc(numNodes, sizeof(**(graph->adjMatrix)));
        if (graph->adjMatrix[i] == NULL) {
            destroyGraph(graph);
            return NULL;
        }
    }
    graph->numNodes = numNodes;
    return graph;
}

void destroyGraph(Graph* graph) {
    for (int i = 0; i < graph->numNodes; ++i) {
        free(graph->adjMatrix[i]);
    }
    free(graph->adjMatrix);
    free(graph);
    graph = NULL;
}

void printGraph(Graph* graph) {
    for (int i = 0; i <= graph->numNodes; ++i) {
        if (i == 0) printf("   ");
        else printf("%2d|", i - 1);
        for (int j = 1; j <= graph->numNodes; ++j) {
            if (i == 0) printf("%3d", j - 1);
            else printf("%3d", graph->adjMatrix[i - 1][j - 1]);
        }
        printf("\n");
        if (!i) {
            printf("   ");
            for (int k = 0; k < graph->numNodes; ++k) {
                printf("---");
            }
            printf("\n");
        }
    }
}

int addEdge(Graph* graph, int from, int to, int value) {
    if (from >= 0 && from < graph->numNodes && to >= 0 && to < graph->numNodes && value > 0) {
        graph->adjMatrix[from][to] = value;
        return 1;
    }
    return 0;
}

int removeEdge(Graph* graph, int from, int to) {
    if (from >= 0 && from < graph->numNodes && to >= 0 && to < graph->numNodes) {
        graph->adjMatrix[from][to] = 0;
        return 1;
    }
    return 0;
}

int addNode(Graph* graph) {
    // Tworzenie nowej macierzy o rozszerzonym rozmiarze
    int** newMatrix = calloc((long long)graph->numNodes + 1, sizeof(*newMatrix));
    if (newMatrix == NULL) {
        return 0;
    }
    for (int i = 0; i < graph->numNodes + 1; ++i) {
        newMatrix[i] = calloc((long long)graph->numNodes + 1, sizeof(**newMatrix));
        if (newMatrix[i] == NULL) {
            for (int j = 0; j < graph->numNodes + 1; ++j) {
                free(newMatrix[j]);
            }
            free(newMatrix);
            newMatrix = NULL;
            return 0;
        }
    }
    // Przenoszenie istniejacych danych do nowej macierzy
    for (int i = 0; i < graph->numNodes; ++i) {
        for (int j = 0; j < graph->numNodes; ++j) {
            newMatrix[i][j] = graph->adjMatrix[i][j];
        }
    }
    // Zwalnianie pamieci poprzedniej macierzy
    for (int i = 0; i < graph->numNodes; ++i) {
        free(graph->adjMatrix[i]);
    }
    free(graph->adjMatrix);
    // Aktualizowanie wskaznikow i rozmiaru
    graph->adjMatrix = newMatrix;
    graph->numNodes++;
    return 1;
}

int removeNode(Graph* graph, int node) {
    if (node >= 0 && node < graph->numNodes) {
        // Tworzenie nowej macierzy o zmniejszonym rozmiarze
        int** newMatrix = calloc((long long)graph->numNodes - 1, sizeof(*newMatrix));
        if (newMatrix == NULL) {
            return 0;
        }
        for (int i = 0; i < graph->numNodes - 1; ++i) {
            newMatrix[i] = calloc((long long)graph->numNodes - 1, sizeof(**newMatrix));
            if (newMatrix[i] == NULL) {
                for (int j = 0; j < graph->numNodes - 1; ++j) {
                    free(newMatrix[j]);
                }
                free(newMatrix);
                newMatrix = NULL;
                return 0;
            }
        }
        // Przenoszenie danych z poprzedniej macierzy do nowej macierzy (bez usuwanego wezla)
        int newRow = 0;
        for (int i = 0; i < graph->numNodes - 1; ++i) {
            if (i == node) {
                continue;
            }
            int newCol = 0;
            for (int j = 0; j < graph->numNodes - 1; ++j) {
                if (j == node) {
                    continue;
                }
                newMatrix[newRow][newCol] = graph->adjMatrix[i][j];
                newCol++;
            }
            newRow++;
        }
        // Zwalnianie pamieci poprzedniej macierzy
        for (int i = 0; i < graph->numNodes; ++i) {
            free(graph->adjMatrix[i]);
        }
        free(graph->adjMatrix);
        // Aktualizowanie wskaznikow i rozmiaru
        graph->adjMatrix = newMatrix;
        graph->numNodes--;
        return 1;
    }
    return 0;
}

int saveGraph(Graph* graph, const char* filename) {
    FILE* file = NULL;
    errno_t error = fopen_s(&file, filename, "w");
    if (error != 0) {
        return 0;
    }
    // Zapis macierzy sasiedztwa do pliku
    for (int i = 0; i < graph->numNodes; ++i) {
        for (int j = 0; j < graph->numNodes; ++j) {
            fprintf(file, "%3d", graph->adjMatrix[i][j]);
        }
        fprintf(file, "\n");
    }
    fclose(file);
    return 1;
}

Graph* loadGraph(const char* filename) {
    FILE* file = NULL;
    if (fopen_s(&file, filename, "r") != 0) {
        return NULL;
    }
    // Obliczanie liczby wezlow na podstawie nowych linii w pliku
    int numNodes = 0;
    char c;
    while ((c = fgetc(file)) != EOF) {
        if (c == '\n') {
            numNodes++;
        }
    }
    // Przesuniecie wskaznika pliku na poczatek
    rewind(file);
    // Tworzenie nowego grafu, do ktorego zapisana zostanie zawartosc pliku
    Graph* graph = createGraph(numNodes);
    // Odczytanie danych z pliku i ich zapis do nowego grafu
    for (int i = 0; i < numNodes; ++i) {
        for (int j = 0; j < numNodes; ++j) {
            if (fscanf_s(file, "%d", &(graph->adjMatrix[i][j])) < 1) {
                fclose(file);
                destroyGraph(graph);
                return NULL;
            }
        }
    }
    fclose(file);
    return graph;
}

void shortestPath(Graph* graph, int startNode) {
    Node* nodes = calloc(graph->numNodes, sizeof(*nodes));
    if (nodes == NULL) {
        return;
    }
    // Inicjalizacja struktury dla kazdego wezla
    for (int i = 0; i < graph->numNodes; ++i) {
        nodes[i].visited = 0;
        nodes[i].distance = INT_MAX;
        nodes[i].previous = -1;
    }
    nodes[startNode].distance = 0;
    for (int i = 0; i < graph->numNodes - 1; ++i) {
        int minDistance = INT_MAX;
        int minNode = -1;
        // Wybor wezla o najmniejszej odleglosci sposrod nieodwiedzonych wezlow
        for (int j = 0; j < graph->numNodes; ++j) {
            if (!nodes[j].visited && nodes[j].distance < minDistance) {
                minDistance = nodes[j].distance;
                minNode = j;
            }
        }
        if (minNode == -1) {
            break;
        }
        nodes[minNode].visited = 1;
        // Aktualizacja odleglosci sasiednich wezlow
        for (int adjNode = 0; adjNode < graph->numNodes; ++adjNode) {
            int weight = graph->adjMatrix[minNode][adjNode];
            if (weight > 0 && !nodes[adjNode].visited && nodes[minNode].distance + weight < nodes[adjNode].distance) {
                nodes[adjNode].distance = nodes[minNode].distance + weight;
                nodes[adjNode].previous = minNode;
            }
        }
    }
    printf("Najkrotsze sciezki z wezla %d:\n", startNode);
    for (int i = 0; i < graph->numNodes; ++i) {
        if (nodes[i].distance == INT_MAX) {
            printf("%d ", i);
            printf("brak sciezki\n");
        }
        else if (i != startNode) {
            int curNode = i;
            printf("%d", curNode);
            while (nodes[curNode].previous != -1) {
                printf(" <- %d", nodes[curNode].previous);
                curNode = nodes[curNode].previous;
            }
            printf(", koszt: %d\n", nodes[i].distance);
        }
    }
    free(nodes);
    nodes = NULL;
}

// Dla grafu acyklicznego
void longestPath(Graph* graph, int startNode) {
    Node* nodes = calloc(graph->numNodes, sizeof(*nodes));
    if (nodes == NULL) {
        return;
    }
    // Inicjalizacja struktury dla ka¿dego wezla
    for (int i = 0; i < graph->numNodes; ++i) {
        nodes[i].distance = INT_MIN;
        nodes[i].previous = -1;
    }
    nodes[startNode].distance = 0;
    // Algorytm znajdowania najdluzszej sciezki
    for (int i = 0; i < graph->numNodes - 1; ++i) {
        for (int j = 0; j < graph->numNodes; ++j) {
            for (int k = 0; k < graph->numNodes; ++k) {
                int weight = graph->adjMatrix[j][k];
                if (weight > 0 && nodes[j].distance != INT_MIN && nodes[j].distance + weight > nodes[k].distance) {
                    nodes[k].distance = nodes[j].distance + weight;
                    nodes[k].previous = j;
                }
            }
        }
    }
    // Sprawdzenie czy istnieje cykl
    for (int i = 0; i < graph->numNodes; ++i) {
        for (int j = 0; j < graph->numNodes; ++j) {
            int weight = graph->adjMatrix[i][j];
            if (weight > 0 && nodes[i].distance != INT_MIN && nodes[i].distance + weight > nodes[j].distance) {
                printf("Graf zawiera cykl. Algorytm dziala tylko dla grafow acyklicznych.\n");
                return;
            }
        }
    }
    printf("Najdluzsze sciezki z wezla %d:\n", startNode);
    for (int i = 0; i < graph->numNodes; ++i) {
        if (nodes[i].distance == INT_MIN) {
            printf("%d ", i);
            printf("brak sciezki\n");
        }
        else if (i != startNode) {
            int curNode = i;
            printf("%d", curNode);
            while (nodes[curNode].previous != -1) {
                printf(" <- %d", nodes[curNode].previous);
                curNode = nodes[curNode].previous;
            }
            printf(", koszt: %d\n", nodes[i].distance);
        }
    }
    free(nodes);
    nodes = NULL;
}

// Funkcja z algorytmem przeszukiwania w glab
int hasCycleDFS(Graph* graph, Node* nodes, int curNode) {
    if (nodes[curNode].stack == 1) {
        return 1;
    }
    if (nodes[curNode].visited == 1) {
        return 0;
    }
    nodes[curNode].visited = 1;
    nodes[curNode].stack = 1;
    for (int adjNode = 0; adjNode < graph->numNodes; ++adjNode) {
        if (graph->adjMatrix[curNode][adjNode] > 0 && hasCycleDFS(graph, nodes, adjNode)) {
            return 1;
        }
    }
    nodes[curNode].stack = 0;
    return 0;
}

int hasCycle(Graph* graph) {
    Node* nodes = calloc(graph->numNodes, sizeof(*nodes));
    if (nodes == NULL) {
        return 0;
    }
    for (int i = 0; i < graph->numNodes; ++i) {
        nodes[i].visited = 0;
        nodes[i].stack = 0;
    }
    for (int i = 0; i < graph->numNodes; ++i) {
        if (hasCycleDFS(graph, nodes, i)) {
            free(nodes);
            nodes = NULL;
            return 1;
        }
    }
    free(nodes);
    nodes = NULL;
    return 0;
}