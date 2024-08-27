#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include "projekt.h"

void menu() {
    Graph* graph;
    while (1) {
        printf("Wybierz jedna z ponizszych opcji:\n\n");
        printf("1. Stworz graf\n");
        printf("2. Wczytaj graf\n");
        printf("0. Zakoncz\n\n");
        printf("Wybor opcji: ");
        fseek(stdin, 0, SEEK_END);
        char choice = _getch();
        switch (choice) {
        case '1':
            printf("\n\nPodaj liczbe wezlow: ");
            int n;
            if (scanf_s("%d", &n) < 1 || n < 1) {
                printf("\nCos poszlo nie tak. Sprobuj ponownie.\n");
                Sleep(1250); system("cls");
                continue;
            }
            graph = createGraph(n);
            if (graph == NULL) {
                printf("\nCos poszlo nie tak. Sprobuj ponownie.\n");
                Sleep(1250); system("cls");
                continue;
            }
            else {
                printf("\nPomyslnie stworzono graf.\n");
            }
            break;
        case '2':
            graph = loadGraph("graf.txt");
            if (graph == NULL) {
                printf("\n\nCos poszlo nie tak. Sprobuj ponownie.\n");
                Sleep(1250); system("cls");
                continue;
            }
            else {
                printf("\n\nPomyslnie wczytano graf.\n");
            }
            break;
        case '0':
            printf("\n\nZamykam...");
            Sleep(1250); system("cls");
            exit(0);
        default:
            printf("\n\nZla opcja! Sprobuj ponownie!\n");
            Sleep(1250); system("cls");
            continue;
        }
        Sleep(1250); system("cls");
        break;
    }
    while (1) {
        printf("Wybierz jedna z ponizszych opcji:\n\n");
        printf("1. Wyswietl graf\n");
        printf("2. Zapisz graf\n");
        printf("3. Dodaj krawedz\n");
        printf("4. Usun krawedz\n");
        printf("5. Dodaj wezel\n");
        printf("6. Usun wezel\n");
        printf("7. Najkrotsze sciezki\n");
        printf("8. Najdluzsze sciezki\n");
        printf("9. Zbadaj cyklicznosc\n");
        printf("0. Zakoncz\n\n");
        printf("Wybor opcji: ");
        fseek(stdin, 0, SEEK_END);
        char c;
        int from, to, value, n;
        char choice = _getch();
        switch (choice) {
        case '1':
            printf("\n\n");
            printGraph(graph);
            printf("\nWcisnij dowolny przycisk, aby kontynuowac...");
            c = _getch(); system("cls");
            continue;
        case '2':
            if (saveGraph(graph, "graf.txt")) {
                printf("\n\nGraf zostal zapisany.\n");
            }
            else {
                printf("\n\nNie udalo sie zapisac grafu.\n");
            }
            break;
        case '3':
            printf("\n\n");
            printf("Od ktorego wezla poprowadzic krawedz? ");
            if (scanf_s("%d", &from) < 1) {
                printf("\nCos poszlo nie tak. Sprobuj ponownie.\n");
                break;
            }
            printf("Do ktorego wezla poprowadzic krawedz? ");
            if (scanf_s("%d", &to) < 1) {
                printf("\nCos poszlo nie tak. Sprobuj ponownie.\n");
                break;
            }
            printf("Podaj wage krawedzi: ");
            if (scanf_s("%d", &value) < 1) {
                printf("\nCos poszlo nie tak. Sprobuj ponownie.\n");
                break;
            }
            if (addEdge(graph, from, to, value)) {
                printf("\nKrawedz zostala dodana do grafu.\n");
            }
            else {
                printf("\nCos poszlo nie tak. Sprobuj ponownie.\n");
            }
            break;
        case '4':
            printf("\n\n");
            printf("Od ktorego wezla usunac krawedz? ");
            if (scanf_s("%d", &from) < 1) {
                printf("\nCos poszlo nie tak. Sprobuj ponownie.\n");
                break;
            }
            printf("Do ktorego wezla usunac krawedz? ");
            if (scanf_s("%d", &to) < 1) {
                printf("\nCos poszlo nie tak. Sprobuj ponownie.\n");
                break;
            }
            if (removeEdge(graph, from, to)) {
                printf("\nKrawedz zostala usunieta z grafu.\n");
            }
            else {
                printf("\nCos poszlo nie tak. Sprobuj ponownie.\n");
            }
            break;
        case '5':
            if (addNode(graph)) {
                printf("\n\nWezel zostal dodany do grafu.\n");
            }
            else {
                printf("\n\nCos poszlo nie tak. Sprobuj ponownie.\n");
            }
            break;
        case '6':
            printf("\n\nKtory wezel usunac? ");
            if (scanf_s("%d", &n) < 1) {
                printf("\nCos poszlo nie tak. Sprobuj ponownie.\n");
                break;
            }
            if (removeNode(graph, n)) {
                printf("\nWezel zostal usuniety z grafu.");
            }
            else {
                printf("\nCos poszlo nie tak. Sprobuj ponownie.\n");
            }
            break;
        case '7':
            printf("\n\nZ ktorego wezla wyznaczyc sciezki? ");
            if (scanf_s("%d", &n) < 1 || n < 0 || n >= graph->numNodes) {
                printf("\nCos poszlo nie tak. Sprobuj ponownie.\n");
                break;
            }
            printf("\n");
            shortestPath(graph, n);
            printf("\nWcisnij dowolny przycisk, aby kontynuowac...");
            c = _getch(); system("cls");
            continue;
        case '8':
            printf("\n\nZ ktorego wezla wyznaczyc sciezki? ");
            if (scanf_s("%d", &n) < 1 || n < 0 || n >= graph->numNodes) {
                printf("\nCos poszlo nie tak. Sprobuj ponownie.\n");
                break;
            }
            printf("\n");
            longestPath(graph, n);
            printf("\nWcisnij dowolny przycisk, aby kontynuowac...");
            c = _getch(); system("cls");
            continue;
        case '9':
            if (hasCycle(graph)) {
                printf("\n\nGraf zawiera cykl.\n");
            }
            else {
                printf("\n\nGraf nie zawiera cyklu.\n");
            }
            printf("\nWcisnij dowolny przycisk, aby kontynuowac...");
            c = _getch(); system("cls");
            continue;
        case '0':
            printf("\n\nZamykam...");
            destroyGraph(graph);
            Sleep(1250); system("cls");
            exit(0);
        default:
            printf("\n\nZla opcja! Sprobuj ponownie!\n");
            break;
        }
        Sleep(1250); system("cls");
    }
}