#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "func4graph.h"

int main()
{
    FILE *file = fopen("idunno.csv", "r");
    if (file == NULL)
    {
        perror("Error opening the file");
        return 1;
    }
    antancs *graph = (antancs *)malloc(sizeof(antancs));
    putdata(graph, file);

    for(int i = 0; i < graph->V; i++)
        for(int j = 0; j<graph->V; j++)
        {
            // if(graph->adjacency_list[i][j] != NULL)
                printf("%s", graph->adjacency_list[i][j]->dst);
        }
    return 0;
}