#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include <math.h>
#include <string.h>

#define MAX_CITIES 50
#define MAX_ANTS 50
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
int start_point = 0; // Define the starting point
int end_point = 10;  // Define the ending point
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
            char src1[] = {src[0], src[1], '\0'};
            char dest1[] = {dest[0], dest[1], '\0'};
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
void init_ant(int k, int start_point, int end_point)
{
    ant[k][0] = start_point; // Set the starting point for each ant
    for (int i = 1; i < num_cities; i++)
    {
        int from = ant[k][i - 1];
        double denom = 0.0;
        for (int j = 0; j < num_cities; j++)
        {
            if (j != end_point && !ant[k][j]) // Exclude the end point from consideration
            {
                denom += pow(pheromone[from][j], ALPHA) * pow(1.0 / distance[from][j], BETA);
            }
        }
        double roulette = rand_double() * denom;
        double prob = 0.0;
        for (int j = 0; j < num_cities; j++)
        {
            if (j != end_point && !ant[k][j]) // Exclude the end point from consideration
            {
                prob += pow(pheromone[from][j], ALPHA) * pow(1.0 / distance[from][j], BETA);
                if (prob >= roulette)
                {
                    ant[k][i] = j;
                    break;
                }
            }
        }
    }
    ant[k][num_cities - 1] = end_point; // Set the ending point for each ant
}

void ant_tour()
{
    for (int i = 0; i < num_ants; i++)
    {
        ant[i][0] = start_point; // Set the starting point for each ant
        init_ant(i, start_point, end_point);
        length[i] = 0.0;
        for (int j = 0; j < num_cities - 1; j++)
        {
            int from = ant[i][j];
            int to = ant[i][j + 1];
            length[i] += distance[from][to];
        }
        length[i] += distance[ant[i][num_cities - 1]][end_point]; // Add the distance to the ending point
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

void update_pheromone()
{
    for (int i = 0; i < num_ants; i++)
    {
        for (int j = 0; j < num_cities - 1; j++)
        {
            int from = ant[i][j];
            int to = ant[i][j + 1];
            if ((from == start_point && to == end_point) || (from == end_point && to == start_point))
            {
                pheromone[from][to] = pheromone[from][to] * (1.0 - RHO) + (QVAL / length[i]) * RHO;
                pheromone[to][from] = pheromone[from][to];
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



int main()
{
    srand(time(NULL));
    num_cities = 43;
    num_ants = 43;
    int start_point = 0; // Define the starting point
    int end_point = 10;  // Define the ending point

    FILE *file = fopen("newer.csv", "r");
    if (file == NULL)
    {
        perror("Error opening the file");
        return 1;
    }
    antancs *graph = (antancs *)malloc(sizeof(antancs));
    graph->V = num_cities;
    graph->adjacency_list = (node **)malloc(graph->V * sizeof(node *));
    for (int i = 0; i < graph->V; i++)
    {
        graph->adjacency_list[i] = (node *)malloc(graph->V * sizeof(node));
    }
    putdata(graph, file);
    for (int i = 0; i < graph->V; i++)
    {
        for (int j = 0; j < graph->V; j++)
        {
            distance[i][j] = graph->adjacency_list[i][j].weight;
        }
    }

    init_pheromone();
    for (int i = 0; i < MAX_TOURS; i++)
    {
        ant_tour(start_point, end_point);
        update_pheromone(start_point, end_point);
    }
    print_path();
    return 0;
}
