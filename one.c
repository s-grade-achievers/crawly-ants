#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define data structures for the network, ants, and pheromone levels.

typedef struct NODE
{
    char src[30];
    char dst[30];
    float weight;
} node;

typedef struct ANTANCS
{
    int V;
    node *adjacency_list[50][50];
} antancs;

typedef struct Ant
{
    int current_node;
    int destination_node;
    float pathcost;
    char pathhistory[50];
}ant;

typedef struct ANTS{
    int num_ants;
    ant *antarray[50];
}antarray;

typedef struct antliq{
    int V;
    node *phmatrix[50][50];
}antliq;

// Initialize network, pheromone levels, and ants.

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
    char *token = malloc(30 * sizeof(char)); // Allocate memory for token
    while (fgets(line, sizeof(line), fp) != NULL)
    {
        if (line[0] != '\n')
        {
            char *src = (char *)malloc(30 * sizeof(char));  // Allocate memory for src
            char *dest = (char *)malloc(30 * sizeof(char)); // Allocate memory for src
            src = strtok(line, ",");
            dest = strtok(NULL, ",");
            float w = atof(strtok(NULL, ","));
            graph->adjacency_list[i][j] = (node *)malloc(sizeof(node));
            strcpy(graph->adjacency_list[i][j]->src, src);
            strcpy(graph->adjacency_list[i][j]->dst, dest);
            graph->adjacency_list[i][j]->weight = w;
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


void initializePheromoneLevels(antliq *pheromone_matrix, antancs *graph)
{
    for(int i = 0; i < pheromone_matrix->V;i++){
        for(int j = 0; j < pheromone_matrix->V; j++){
            graph->adjacency_list[i][j] = (node *)malloc(sizeof(node));
            pheromone_matrix->phmatrix[i][j] = graph->adjacency_list[i][j];
            pheromone_matrix->phmatrix[i][j]->weight = 1.0;
        }
    }
}

//ants will also have history and pathcost 
void initializeAnts(antarray *ants)
{
    for(int i = 0; i <ants->num_ants ; i++){
        ants->antarray[i] = (ant *)malloc(sizeof(ant));
        ants->antarray[i]->pathcost = 0; 
    }
}

// // Main ACO algorithm.

// void acoAlgorithm(struct Node *nodes, int num_nodes, struct Link *links, int num_links, struct Ant *ants, int num_ants)
// {
//     int max_iterations = 100;
//     double evaporation_rate = 0.1;
//     double alpha = 1.0; // Pheromone influence.
//     double beta = 2.0;  // Heuristic influence.

//     for (int iteration = 1; iteration <= max_iterations; iteration++)
//     {
//         // Move ants, update pheromone levels, and perform other ACO operations.
//     }
// }

int main()
{
    FILE *file = fopen("new.csv", "r");
    if (file == NULL)
    {
        perror("Error opening the file");
        return 1;
    }
    antancs *graph = (antancs *)malloc(sizeof(antancs));
    graph->V = 50;
    putdata(graph, file);
    antliq *phamtrix = (antliq *)malloc(sizeof(antliq));
    phamtrix->V = 50;
    initializePheromoneLevels(phamtrix, graph);
    ants


    return 0;
}