#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>


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

typedef struct
{
    int NUMBEROFANTS;
    int NUMBEROFCITIES;
    double ALPHA;
    double BETA;
    double Q;
    double RO;
    double TAUMAX;
    int INITIALCITY;
    int **GRAPH;
    double **CITIES;
    double **PHEROMONES;
    double **DELTAPHEROMONES;
    double **PROBS;
    int **ROUTES;
    double BESTLENGTH;
    int *BESTROUTE;
} ACO;

antancs *putdata(antancs *graph, FILE *fp, ACO* aco)
{
    if (fp == NULL)
    {
        printf("Error opening the file");
        return graph;
    }

    char line[300];
    fgets(line, sizeof(line), fp);

    int i = 0, j = 0;
    char *token = malloc(30 * sizeof(char)); 
    while (fgets(line, sizeof(line), fp) != NULL)
    {
        if (line[0] != '\n')
        {
            char *src = (char *)malloc(30 * sizeof(char));  
            char *dest = (char *)malloc(30 * sizeof(char)); 
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

ACO *initACO(int nAnts, int nCities, double alpha, double beta, double q, double ro, double taumax, int initCity, Randoms *randoms)
{
    ACO *aco = malloc(sizeof(ACO));
    aco->NUMBEROFANTS = nAnts;
    aco->NUMBEROFCITIES = nCities;
    aco->ALPHA = alpha;
    aco->BETA = beta;
    aco->Q = q;
    aco->RO = ro;
    aco->TAUMAX = taumax;
    aco->INITIALCITY = initCity;
    aco->BESTLENGTH = INT_MAX;

    aco->GRAPH = (int **)malloc(nCities * sizeof(int *));
    aco->CITIES = (double **)malloc(nCities * sizeof(double *));
    aco->PHEROMONES = (double **)malloc(nCities * sizeof(double *));
    aco->DELTAPHEROMONES = (double **)malloc(nCities * sizeof(double *));
    aco->PROBS = (double **)malloc((nCities - 1) * sizeof(double *));
    aco->ROUTES = (int **)malloc(nAnts * sizeof(int *));
    aco->BESTROUTE = (int *)malloc(nCities * sizeof(int));

    for (int i = 0; i < nCities; i++)
    {
        aco->GRAPH[i] = (int *)malloc(nCities * sizeof(int));
        aco->CITIES[i] = (double *)malloc(2 * sizeof(double));
        aco->PHEROMONES[i] = (double *)malloc(nCities * sizeof(double));
        aco->DELTAPHEROMONES[i] = (double *)malloc(nCities * sizeof(double));
        if (i < nCities - 1)
        {
            aco->PROBS[i] = (double *)malloc(2 * sizeof(double));
        }
        for (int j = 0; j < 2; j++)
        {
            aco->CITIES[i][j] = -1.0;
            aco->PROBS[i][j] = -1.0;
        }
        for (int j = 0; j < nCities; j++)
        {
            aco->GRAPH[i][j] = 0;
            aco->PHEROMONES[i][j] = 0.0;
            aco->DELTAPHEROMONES[i][j] = 0.0;
        }
    }

    for (int i = 0; i < nAnts; i++)
    {
        aco->ROUTES[i] = (int *)malloc(nCities * sizeof(int));
        for (int j = 0; j < nCities; j++)
        {
            aco->ROUTES[i][j] = -1;
        }
    }

    for (int i = 0; i < nCities; i++)
    {
        aco->BESTROUTE[i] = -1;
    }

    return aco;
}

int main()
{
    FILE *file = fopen("newer.csv", "r");
    if (file == NULL)
    {
        perror("Error opening the file");
        return 1;
    }
    antancs *graph = (antancs *)malloc(sizeof(antancs));
    graph->V = 50;
    putdata(graph, file);
    return 0;
}