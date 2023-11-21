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
    node **adjacency_list;
} antancs;

char **cities;
int minDistance(float dist[], int sptSet[], int V)
{
    float min = INT_MAX;
    int min_index;

    for (int v = 0; v < V; v++)
        if (sptSet[v] == 0 && dist[v] <= min)
            min = dist[v], min_index = v;

    return min_index;
}

void printPath(int parent[], int j)
{
    if (parent[j] == -1)
        return;

    printPath(parent, parent[j]);

    printf("%d ", j);
}

void printSolution(float dist[], int parent[], int src, int V, int dest)
{
    printf("Vertex   Distance from Source   Path\n");
    printf("%d \t\t %.2f \t %d ", src, dist[dest], src);
    printPath(parent, dest);
    printf("\n");
}

void dijkstra(antancs *graph, int src, int dest)
{
    int V = graph->V;
    float dist[V];
    int parent[V];

    for (int i = 0; i < V; i++)
    {
        dist[i] = INT_MAX;
        parent[i] = -1;
    }

    int sptSet[V];

    for (int i = 0; i < V; i++)
        sptSet[i] = 0;

    dist[src] = 0;

    for (int count = 0; count < V - 1; count++)
    {
        int u = minDistance(dist, sptSet, V);
        sptSet[u] = 1;

        for (int v = 0; v < V; v++)
        {
            if (!sptSet[v] && graph->adjacency_list[u][v].weight && dist[u] != INT_MAX && dist[u] + graph->adjacency_list[u][v].weight < dist[v])
            {
                dist[v] = dist[u] + graph->adjacency_list[u][v].weight;
                parent[v] = u;
            }
        }
    }

    
    printSolution(dist, parent, src, V, dest);
}

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
            graph->adjacency_list[result1][result].weight = w;
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
    FILE *fp = fopen("newer.csv", "r");
    antancs *graph = (antancs *)malloc(sizeof(antancs));
    graph->V = 100;
    graph->adjacency_list = (node **)malloc(graph->V * sizeof(node *));
    for (int i = 0; i < graph->V; i++)
        graph->adjacency_list[i] = (node *)malloc(graph->V * sizeof(node));

    graph = putdata(graph, fp);

    FILE *f = fopen("cities.txt", "r");
    cities = (char **)malloc(sizeof(char *) * 43);
    for (int i = 0; i < 43; i++)
        cities[i] = (char *)malloc(sizeof(char));
    char line[40];
    int i = 0;
    while (fgets(line, sizeof(line), f))
    {
        char *src = strtok(line, "\n");
        strcpy(cities[i], src);
        i++;
    }

    int src = 0, dest = 0;
    printf("Enter the source and destination vertices: ");
    scanf("%d %d", &src, &dest);

    dijkstra(graph, src, dest);

    return 0;
}
