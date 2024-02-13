#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

#define MAX_CITIES 20  // Adjust the maximum number of cities

struct Graph {
    int numVertices;
    int** adjacencyMatrix;
};

struct Flight {
    char cities[MAX_CITIES][50];
    int numCities;
};

// Function prototypes
void initializeGraph(struct Graph* graph, int numVertices);
void addFlight(struct Graph* graph, const struct Flight* flights, int source, int destination, int distance);
void displayFlights(const struct Graph* graph, const struct Flight* flights);
void displayCities(const struct Flight* flights);
void dijkstra(const struct Graph* graph, int source, int destination, const struct Flight* flights);

void printMenu();
void printError(const char* message);

int main() {
    struct Graph graph;
    struct Flight flights;

    // Initialize city names and flight data for major cities in India
    strcpy(flights.cities[0], "Mumbai");
    strcpy(flights.cities[1], "Pune");
    strcpy(flights.cities[2], "Delhi");
    strcpy(flights.cities[3], "Ahmedabad");
    strcpy(flights.cities[4], "Surat");
    strcpy(flights.cities[5], "Chennai");
    strcpy(flights.cities[6], "Kolkata");
    strcpy(flights.cities[7], "Bangalore");
    strcpy(flights.cities[8], "Hyderabad");
    flights.numCities = 9;

    initializeGraph(&graph, flights.numCities);
    addFlight(&graph, &flights, 0, 1, 150);
    addFlight(&graph, &flights, 0, 2, 1100);
    addFlight(&graph, &flights, 0, 3, 500);
    addFlight(&graph, &flights, 0, 4, 600);
    addFlight(&graph, &flights, 0, 5, 1200);
    addFlight(&graph, &flights, 0, 6, 1700);
    addFlight(&graph, &flights, 0, 7, 800);
    addFlight(&graph, &flights, 0, 8, 700);
    addFlight(&graph, &flights, 1, 2, 1100);
    addFlight(&graph, &flights, 1, 3, 200);
    addFlight(&graph, &flights, 1, 4, 300);
    addFlight(&graph, &flights, 1, 5, 1000);
    addFlight(&graph, &flights, 1, 6, 1500);
    addFlight(&graph, &flights, 1, 7, 100);
    addFlight(&graph, &flights, 1, 8, 1000);
    addFlight(&graph, &flights, 2, 3, 900);
    addFlight(&graph, &flights, 2, 4, 1000);
    addFlight(&graph, &flights, 2, 5, 1400);
    addFlight(&graph, &flights, 2, 6, 1500);
    addFlight(&graph, &flights, 2, 7, 2000);
    addFlight(&graph, &flights, 2, 8, 1500);
    addFlight(&graph, &flights, 3, 4, 100);
    addFlight(&graph, &flights, 3, 5, 1100);
    addFlight(&graph, &flights, 3, 6, 1600);
    addFlight(&graph, &flights, 3, 7, 700);
    addFlight(&graph, &flights, 3, 8, 900);
    addFlight(&graph, &flights, 4, 5, 1100);
    addFlight(&graph, &flights, 4, 6, 1600);
    addFlight(&graph, &flights, 4, 7, 800);
    addFlight(&graph, &flights, 4, 8, 900);
    addFlight(&graph, &flights, 5, 6, 1400);
    addFlight(&graph, &flights, 5, 7, 2000);
    addFlight(&graph, &flights, 5, 8, 1000);
    addFlight(&graph, &flights, 6, 7, 1500);
    addFlight(&graph, &flights, 6, 8, 1200);
    addFlight(&graph, &flights, 7, 8, 600);

    // Menu
    int choice;
    do {
        printf("\nFlight Route Planner Menu:\n");
        printMenu();
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("\nAvailable Flights:\n");
                displayFlights(&graph, &flights);
                break;
            case 2: {
                printf("\nList of Available Cities:\n");
                displayCities(&flights);
                int source, destination;
                printf("\nEnter source city index: ");
                scanf("%d", &source);
                printf("Enter destination city index: ");
                scanf("%d", &destination);
                dijkstra(&graph, source, destination, &flights);
                break;
            }
            case 3:
                printf("\nExiting program.\n");
                break;
            default:
                printError("Invalid choice. Please enter a valid option.");
        }

    } while (choice != 3);

    // Deallocate dynamic memory (if any)
    for (int i = 0; i < graph.numVertices; i++) {
        free(graph.adjacencyMatrix[i]);
    }
    free(graph.adjacencyMatrix);

    return 0;
}

void initializeGraph(struct Graph* graph, int numVertices) {
    graph->numVertices = numVertices;

    // Allocate memory for adjacency matrix
    graph->adjacencyMatrix = (int**)malloc(numVertices * sizeof(int*));
    for (int i = 0; i < numVertices; i++) {
        graph->adjacencyMatrix[i] = (int*)malloc(numVertices * sizeof(int));
    }

    // Initialize distances to infinity
    for (int i = 0; i < numVertices; i++) {
        for (int j = 0; j < numVertices; j++) {
            graph->adjacencyMatrix[i][j] = INT_MAX;
        }
    }
}

void addFlight(struct Graph* graph, const struct Flight* flights, int source, int destination, int distance) {
    if (source >= 0 && source < flights->numCities && destination >= 0 && destination < flights->numCities) {
        graph->adjacencyMatrix[source][destination] = distance;
        graph->adjacencyMatrix[destination][source] = distance; // Since flights are bi-directional
    } else {
        printError("Invalid source or destination city.");
    }
}

void displayFlights(const struct Graph* graph, const struct Flight* flights) {
    for (int i = 0; i < flights->numCities; i++) {
        for (int j = 0; j < flights->numCities; j++) {
            if (graph->adjacencyMatrix[i][j] != INT_MAX && i != j) {
                printf("%s to %s: %d miles\n", flights->cities[i], flights->cities[j], graph->adjacencyMatrix[i][j]);
            }
        }
    }
}

void displayCities(const struct Flight* flights) {
    printf("List of Available Cities:\n");
    for (int i = 0; i < flights->numCities; i++) {
        printf("%d. %s\n", i, flights->cities[i]);
    }
}

void dijkstra(const struct Graph* graph, int source, int destination, const struct Flight* flights) {
    int numVertices = graph->numVertices;
    int* distance = (int*)malloc(numVertices * sizeof(int));
    int* visited = (int*)malloc(numVertices * sizeof(int));

    for (int i = 0; i < numVertices; i++) {
        distance[i] = INT_MAX;
        visited[i] = 0;
    }

    distance[source] = 0;

    for (int count = 0; count < numVertices - 1; count++) {
        int minDistance = INT_MAX;
        int minIndex = -1;
        for (int v = 0; v < numVertices; v++) {
            if (!visited[v] && distance[v] <= minDistance) {
                minDistance = distance[v];
                minIndex = v;
            }
        }

        if (minIndex == -1) break;

        visited[minIndex] = 1;

        for (int v = 0; v < numVertices; v++) {
            if (!visited[v] && graph->adjacencyMatrix[minIndex][v] && distance[minIndex] != INT_MAX
                && distance[minIndex] + graph->adjacencyMatrix[minIndex][v] < distance[v]) {
                distance[v] = distance[minIndex] + graph->adjacencyMatrix[minIndex][v];
            }
        }
    }

    printf("\nShortest route from %s to %s: %d miles\n", flights->cities[source], flights->cities[destination], distance[destination]);

    free(distance);
    free(visited);
}

void printMenu() {
    printf("1. Display Flights\n");
    printf("2. Find Shortest Route\n");
    printf("3. Exit\n");
}

void printError(const char* message) {
    printf("Error: %s\n", message);
}
