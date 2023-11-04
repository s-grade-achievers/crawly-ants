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
    return 0;
}