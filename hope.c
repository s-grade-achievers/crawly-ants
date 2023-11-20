#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include <math.h>
#include <string.h>

#define MAX_CITIES 43
#define MAX_ANTS 43
#define ALPHA 1.0
#define BETA 5.0
#define RHO 0.5
#define QVAL 100
#define MAX_TOURS 100
#define MAX_TIME (MAX_TOURS * MAX_CITIES)
#define INIT_PHEROMONE (1.0 / MAX_CITIES)

double distance[MAX_CITIES][MAX_CITIES];
double pheromone[MAX_CITIES][MAX_CITIES];
double best_path[MAX_CITIES];
double best_length = INT_MAX;

int num_cities;
int num_ants;
int ant[MAX_ANTS][MAX_CITIES];
double length[MAX_ANTS];

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

double rand_double()
{
    return (double)rand() / RAND_MAX;
}

void init_pheromone()
{
    for (int i = 0; i < num_cities; i++)
    {
        for (int j = 0; j < num_cities; j++)
        {
            pheromone[i][j] = INIT_PHEROMONE;
        }
    }
}

void init_ant(int k)
{
    ant[k][0] = 10;
    for (int i = 1; i < num_cities; i++)
    {
        int j;
        double denom = 0.0;
        for (j = 0; j < num_cities; j++)
        {
            if (!ant[k][j])
            {
                denom += pow(pheromone[ant[k][j - 1]][j], ALPHA) * pow(1.0 / distance[ant[k][j - 1]][j], BETA);
            }
        }
        for (j = 0; j < num_cities; j++)
        {
            if (!ant[k][j])
            {
                double numerator = pow(pheromone[ant[k][j - 1]][j], ALPHA) * pow(1.0 / distance[ant[k][j - 1]][j], BETA);
                double prob = numerator / denom;
                if (rand_double() < prob)
                {
                    ant[k][i] = j;
                    break;
                }
            }
        }
    }
}

void update_pheromone()
{
    for (int i = 0; i < num_ants; i++)
    {
        for (int j = 0; j < num_cities - 1; j++)
        {
            int from = ant[i][j];
            int to = ant[i][j + 1];
            pheromone[from][to] = pheromone[from][to] * (1.0 - RHO) + (QVAL / length[i]) * RHO;
            pheromone[to][from] = pheromone[from][to];
        }
    }
}

void ant_tour()
{
    for (int i = 0; i < num_ants; i++)
    {
        init_ant(i);
        length[i] = 0.0;
        for (int j = 0; j < num_cities - 1; j++)
        {
            int from = ant[i][j];
            int to = ant[i][j + 1];
            length[i] += distance[from][to];
        }
        length[i] += distance[ant[i][num_cities - 1]][ant[i][0]];
        if (length[i] < best_length)
        {
            best_length = length[i];
            for (int j = 0; j < num_cities; j++)
            {
                best_path[j] = ant[i][j];
            }
        }
    }
}

void print_path()
{
    printf("Best path: ");
    for (int i = 0; i < num_cities; i++)
    {
        printf("%d ", (int)best_path[i]);
    }
    printf("\nBest length: %f\n", best_length);
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
    char *token = malloc(30 * sizeof(char));
    while (fgets(line, sizeof(line), fp) != NULL)
    {
        if (line[0] != '\n')
        {
            char *src = (char *)malloc(30 * sizeof(char));
            char *dest = (char *)malloc(30 * sizeof(char));
            src = strtok(line, ",");
            dest = strtok(NULL, ",");
            char src1[] = {src[0], src[1], '\0'};
            char dest1[] = {dest[0], dest[1], '\0'};
            // connectcities(src1, dest1, aco, r);
            float w = atof(strtok(NULL, ","));
            strcpy(graph->adjacency_list[i][j].src, src);
            strcpy(graph->adjacency_list[i][j].dst, dest);
            graph->adjacency_list[i][j].weight = w;
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
    srand(time(NULL));
    num_cities = 43; // Change this to the number of cities in your problem
    num_ants = 43;   // Change this to the number of ants in your problem
    // Initialize distance matrix with your problem data
    // Initialize pheromone matrix

    FILE *file = fopen("newer.csv", "r");
    if (file == NULL)
    {
        perror("Error opening the file");
        return 1;
    }
    antancs *graph = (antancs *)malloc(sizeof(antancs));
    graph->V = 50;
    graph->adjacency_list = (node **)malloc(graph->V * sizeof(node *));
    for (int i = 0; i < graph->V; i++)
    {
        graph->adjacency_list[i] = (node *)malloc(graph->V * sizeof(node));
    }
    putdata(graph, file);
    // printf("%s %s %f", graph->adjacency_list[0][0].src, graph->adjacency_list[0][0].dst, graph->adjacency_list[0][0].weight);
    for (int i = 0; i < graph->V; i++)
    {
        for (int j = 0; j < graph->V; j++)
        {
            // Assuming the distance between cities i and j is stored in the weight field of graph->adjacency_list[i][j]
            distance[i][j] = graph->adjacency_list[i][j].weight;
        }
    }

    init_pheromone();
    for (int i = 0; i < MAX_TOURS; i++)
    {
        ant_tour();
        update_pheromone();
    }
    print_path();
    return 0;
}
