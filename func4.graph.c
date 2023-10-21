#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct NODE
{
    char src[30];
    char dst[30];
    float weight;
} node;

typedef struct ANTANCS
{
    int V;
    struct NODE *adjacency_list[50][50];
} antancs;

// antancs *insertedge(antancs *graph, int i, int j, const char to[20], const char from[20], double weight)
// {
//     node *new_node = (node *)malloc(sizeof(node));
//     if (new_node == NULL)
//     {

//         printf("Memory allocation error\n");
//         return graph;
//     }

//     strcpy(new_node->src, from);
//     strcpy(new_node->dst, to);
//     new_node->weight = weight;

//     graph->adjacency_list[i][j] = new_node;
//     return graph;
// }

antancs *putdata(antancs *graph, FILE *fp)
{
    if (fp == NULL)
    {
        printf("Error opening the file");
        return graph;
    }

    char line[300];
    fgets(line, 50, fp);
    // for (int i = 0; i < graph->V; i++)
    // {
    //     for (int j = 0; j < graph->V; j++)
    //     {
    //         if (fgets(line, sizeof(line), fp))
    //         {
    //             int column_count = 0;
    //             char *token = strtok(line, ",");
    //             char to[20];
    //             char from[20];
    //             double weight = 0;
    //             while (token != NULL)
    //             {
    //                 column_count++;
    //                 if (column_count == 1)
    //                 {
    //                     strncpy(to, token, sizeof(to));
    //                 }
    //                 else if (column_count % 2 == 0)
    //                 {
    //                     strncpy(from, token, sizeof(from));
    //                 }
    //                 else if (column_count % 3 == 0)
    //                 {
    //                     weight = strtod(token, NULL);
    //                 }
    //                 token = strtok(NULL, ",");
    //             }
    //             insertedge(graph, i, j, to, from, weight);
    //         }
    //     }
    // }
    int i = 0, j = 0;
    char *token = malloc(30 * sizeof(char)); // Allocate memory for token
    while (fgets(line, sizeof(line), fp) != NULL)
    {
        char *src = (char *)malloc(30 * sizeof(char)); // Allocate memory for src
        strcpy(token, "test");
        src = strtok(line, ",");
        while (token != NULL)
        {
            graph->adjacency_list[i][j] = (node *)malloc(sizeof(node));
            token = strtok(NULL, ",");
            strcpy(graph->adjacency_list[i][j]->src, src);
            strcpy(graph->adjacency_list[i][j]->dst, token);
            token = strtok(NULL, ",");
            graph->adjacency_list[i][j]->weight = atof(token);
            j++;
        }
        i++;
        j = 0;
        free(src);
    }
    return graph;
}

int main()
{
    FILE *file = fopen("dataset.csv", "r");
    if (file == NULL)
    {
        perror("Error opening the file");
        return 1;
    }
    antancs *graph = (antancs *)malloc(sizeof(antancs));
    graph->V = 50;
    putdata(graph, file);

    // for (int i = 0; i < graph->V; i++)
    //     for (int j = 0; j < graph->V; j++)
    //     {
    //         if (graph->adjacency_list[i][j] != NULL)
    //             printf("%s\n", graph->adjacency_list[i][j]->src);
    //     }
    return 0;
}