#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct NODE
{
    char src[20];
    char dst[20];
    double weight;
} node;

typedef struct ANTANCS
{
    int V;
    struct NODE *adjacency_list[50][50];
} antancs;

antancs *insertedge(antancs *graph, int i, int j, const char to[20], const char from[20], double weight)
{
    node *new_node = (node *)malloc(sizeof(node));
    if (new_node == NULL)
    {

        printf("Memory allocation error\n");
        return graph;
    }

    strcpy(new_node->src, from);
    strcpy(new_node->dst, to);
    new_node->weight = weight;

    graph->adjacency_list[i][j] = new_node;
    return graph;
}

antancs *putdata(antancs *graph, FILE *fp)
{
    if (fp == NULL)
    {
        printf("Error opening the file");
        return graph;
    }

    char line[50];
    for (int i = 0; i < graph->V; i++)
    {
        for (int j = 0; j < graph->V; j++)
        {
            if (fgets(line, sizeof(line), fp))
            {
                int column_count = 0;
                char *token = strtok(line, ",");
                char to[20];
                char from[20];
                double weight = 0;
                while (token != NULL)
                {
                    column_count++;
                    if (column_count == 1)
                    {
                        strncpy(to, token, sizeof(to));
                    }
                    else if (column_count == 2)
                    {
                        strncpy(from, token, sizeof(from));
                    }
                    else if (column_count == 3)
                    {
                        weight = strtod(token, NULL);
                    }
                    token = strtok(NULL, ",");
                }
                insertedge(graph, i, j, to, from, weight);
            }
        }
    }
    return graph;
}
