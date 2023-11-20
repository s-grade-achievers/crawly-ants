// Pseudocode for Ant Colony Optimization (ACO) in C
#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    int numNodes;            // Number of nodes in the graph
    double **distance;       // 2D array to store the distance between nodes
    double **pheromone;      // 2D array to store the pheromone level between nodes
    double globalBestLength; // Length of the global best tour
    int *globalBestTour;     // Array to store the global best tour
    // Add other attributes as needed for the ACO algorithm
} Graph;

typedef struct
{
    int currentNode;   // Current node the ant is located at
    int *tour;         // Array to store the tour constructed by the ant
    double tourLength; // Length of the tour constructed by the ant
    // Add other attributes as needed for the ACO algorithm
} Ant;

typedef struct NODE
{
    char src[30];
    char dst[30];
    float weight;
} node;

typedef struct ANTANCS
{
    int V;
    node **adjacency_list;
} antancs;

antancs *putdata(antancs *graph, FILE *fp)
{
    if (fp == NULL)
    {
        printf("Error opening the file");
        return graph;
    }

    char line[300];
    fgets(line, sizeof(line), fp);

    int i = 0, j = 0;
    while (fgets(line, sizeof(line), fp) != NULL)
    {
        if (line[0] != '\n')
        {
            char *src = strtok(line, ",");
            char *dest = strtok(NULL, ",");
            int result1 = (src[0] - '0') * 10 + (src[1] - '0');
            int result = (dest[0] - '0') * 10 + (dest[1] - '0');
            float w = atof(strtok(NULL, ","));
            strcpy(graph->adjacency_list[result1][result].src, src);
            strcpy(graph->adjacency_list[result1][result].dst, dest);
            graph->adjacency_list[i][result].weight = w;
            j++;
        }
        else
        {
            i++;
            j = 0;
        }
    }
    return graph;
}

Ant *createAnts(int numAnts, int numNodes)
{
    Ant *ants = (Ant *)malloc(numAnts * sizeof(Ant)); // Allocate memory for the ants
    for (int i = 0; i < numAnts; i++)
    {
        // Initialize each ant with necessary attributes
        ants[i].currentNode = 0; // Start from the first node
        ants[i].tourLength = 0;  // Initialize tour length
        // Allocate memory for the tour array based on the number of nodes
        ants[i].tour = (int *)malloc(numNodes * sizeof(int));
        // Initialize other attributes of the ant
    }
    return ants;
}

// Define the ACO algorithm functions
void initializePheromoneLevels(Graph *graph, double initialPheromoneLevel)
{
    // Initialize pheromone levels on the graph
    for (int i = 0; i < graph->numNodes; i++)
    {
        for (int j = 0; j < graph->numNodes; j++)
        {
            graph->pheromone[i][j] = initialPheromoneLevel;
        }
    }
}

void updatePheromoneLevels(Graph *graph, Ant *ants, double evaporationRate, double pheromoneDeposit)
{
    // Update pheromone levels based on ant movement
    for (int i = 0; i < graph->numNodes; i++)
    {
        for (int j = 0; j < graph->numNodes; j++)
        {
            // Evaporate pheromone on all edges
            graph->pheromone[i][j] *= (1.0 - evaporationRate);
        }
    }

    for (int k = 0; k < numAnts; k++)
    {
        double antPheromoneDeposit = pheromoneDeposit / ants[k].tourLength; // Adjust pheromone deposit based on ant's tour length
        for (int i = 0; i < graph->numNodes - 1; i++)
        {
            int from = ants[k].tour[i];
            int to = ants[k].tour[i + 1];
            graph->pheromone[from][to] += antPheromoneDeposit; // Deposit pheromone on the path taken by the ant
            graph->pheromone[to][from] += antPheromoneDeposit; // Update pheromone for both directions in undirected graph
        }
    }
}

void updateGlobalBestSolution(Graph *graph, Ant *ants, int numAnts)
{
    // Find the ant with the best solution (e.g., shortest path)
    int bestAntIndex = 0;
    double shortestPathLength = ants[0].tourLength;

    for (int k = 1; k < numAnts; k++)
    {
        if (ants[k].tourLength < shortestPathLength)
        {
            shortestPathLength = ants[k].tourLength;
            bestAntIndex = k;
        }
    }

    // Update the global best solution if a better solution is found
    if (shortestPathLength < graph->globalBestLength)
    {
        graph->globalBestLength = shortestPathLength;
        // Update the global best tour based on the best ant's tour
        for (int i = 0; i < graph->numNodes; i++)
        {
            graph->globalBestTour[i] = ants[bestAntIndex].tour[i];
        }
    }
}

void moveAntToConstructSolution(Ant *ant, Graph *graph, double alpha, double beta)
{
    // Initialize the ant's tour
    ant->tour[0] = ant->currentNode; // Start from the current node
    // Move the ant to construct a solution based on pheromone levels and heuristic information
    for (int i = 1; i < graph->numNodes; i++)
    {
        int nextNode = selectNextNode(ant, graph, alpha, beta);         // Select the next node based on pheromone levels and heuristic information
        ant->tour[i] = nextNode;                                        // Move to the next node
        ant->tourLength += graph->distance[ant->currentNode][nextNode]; // Update the tour length
        ant->currentNode = nextNode;                                    // Update the current node
    }
    // Complete the tour by returning to the starting node
    ant->tourLength += graph->distance[ant->tour[graph->numNodes - 1]][ant->tour[0]];
}

void antColonyOptimization(Graph *graph, int numAnts, int numIterations, double evaporationRate, double alpha, double beta)
{
    // Initialize pheromone levels
    initializePheromoneLevels(graph, initialPheromoneLevel);

    for (int iter = 0; iter < numIterations; iter++)
    {
        // Create ants
        Ant *ants = createAnts(numAnts);

        // Move ants and update pheromone levels
        for (int k = 0; k < numAnts; k++)
        {
            // Move ant k to construct a solution
            moveAntToConstructSolution(&ants[k], graph, alpha, beta);
            // Update pheromone levels based on ant k's tour
            updatePheromoneLevels(graph, &ants[k], evaporationRate, pheromoneDeposit);
        }

        // Update global best solution if necessary
        updateGlobalBestSolution(graph, ants, numAnts);

        // Release memory for ants
        releaseAnts(ants);
    }
}

int main()
{
    // Create the graph

    FILE *fp = fopen("newer.csv", "r"); // Open the CSV file for reading
    if (fp == NULL)
    {
        printf("Error opening the file");
        return 1;
    }

    // Create the antancs structure
    antancs *graph = (antancs *)malloc(sizeof(antancs));
    graph->V = 10; // Set the number of nodes (adjust as per your requirement)

    // Allocate memory for the adjacency list
    graph->adjacency_list = (node **)malloc(graph->V * sizeof(node *));
    for (int i = 0; i < graph->V; i++)
    {
        graph->adjacency_list[i] = (node *)malloc(graph->V * sizeof(node));
    }

    // Populate the graph using the putdata function
    graph = putdata(graph, fp);

    // Close the file
    fclose(fp);

    Graph *graph1 = (Graph*)malloc(sizeof(Graph));
    graph1->numNodes = 43;
    graph1->distance = (double**)malloc(sizeof(double) * graph1->numNodes * graph1->numNodes);
    graph1->pheromone = (double**)malloc(sizeof(double) * graph1->numNodes * graph1->numNodes);
    graph1->globalBestTour = (int*)malloc(sizeof(int) * graph1->numNodes);
    graph1->globalBestLength = 10000000000.0;
    for (int i = 0; i < graph->V; i++)
    {
        for (int j = 0; j < graph->V; j++)
        {
            graph1->distance[i][j] = graph->adjacency_list[i][j].weight;
        }
    }
    double initialPheromoneLevel = 0.1;
    initializePheromoneLevels(graph1, initialPheromoneLevel);
    // Create the ants
    Ant *ants = createAnts(numAnts);
    // Start ACO with 100 iterations
    int numIterations = 100;
    antColonyOptimization(graph1, numAnts, numIterations, 0.5, 1.0, 2.0);
    return 0;
}
