#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>

#define MAX_CITIES 43
#define MAX_ANTS 43
#define ALPHA 1.0
#define BETA 2.0
#define RHO 0.5
#define QVAL 100

int ants[MAX_ANTS][MAX_CITIES];

double distance[MAX_CITIES][MAX_CITIES];
double pheromone[MAX_CITIES][MAX_CITIES];
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

void initialize_pheromone()
{
    // Initialize pheromone levels on each edge to a small value
    for (int i = 0; i < MAX_CITIES; i++)
    {
        for (int j = 0; j < MAX_CITIES; j++)
        {
            pheromone[i][j] = 0.01;
        }
    }
}

void generate_ants(int start_point, int end_point)
{
    // Generate a population of artificial ants
    for (int i = 0; i < MAX_ANTS; i++)
    {
        ants[i][0] = start_point;
        for (int j = 1; j < MAX_CITIES; j++)
        {
            int current_city = ants[i][j - 1];
            double total = 0.0;
            double probabilities[MAX_CITIES];
            for (int k = 0; k < MAX_CITIES; k++)
            {
                if (k == current_city)
                {
                    probabilities[k] = 0.0;
                }
                else
                {
                    double pheromone_level = pheromone[current_city][k];
                    double distance_to_city = distance[current_city][k];
                    double probability = pow(pheromone_level, ALPHA) * pow(1.0 / distance_to_city, BETA);
                    probabilities[k] = probability;
                    total += probability;
                }
            }
            double random_number = ((double)rand() / RAND_MAX) * total;
            double cumulative_probability = 0.0;
            int next_city = -1;
            for (int k = 0; k < MAX_CITIES; k++)
            {
                if (k == current_city)
                {
                    continue;
                }
                cumulative_probability += probabilities[k];
                if (random_number <= cumulative_probability)
                {
                    next_city = k;
                    break;
                }
            }
            ants[i][j] = next_city;
            if (next_city == end_point)
            {
                break;
            }
        }
    }
}

int select_next_node(int current_node, int end_point)
{
    double total = 0.0;
    double probabilities[MAX_CITIES];
    // Calculate the total probability of all unvisited nodes
    for (int i = 0; i < MAX_CITIES; i++)
    {
        if (i != current_node && i != end_point)
        {
            total += pow(pheromone[current_node][i], ALPHA) * pow(1.0 / distance[current_node][i], BETA);
        }
    }
    // Calculate the probability of each unvisited node
    for (int i = 0; i < MAX_CITIES; i++)
    {
        if (i != current_node && i != end_point)
        {
            probabilities[i] = pow(pheromone[current_node][i], ALPHA) * pow(1.0 / distance[current_node][i], BETA) / total;
        }
        else
        {
            probabilities[i] = 0.0;
        }
    }
    // Select the next node probabilistically
    double random = (double)rand() / RAND_MAX;
    double cumulative_probability = 0.0;
    for (int i = 0; i < MAX_CITIES; i++)
    {
        cumulative_probability += probabilities[i];
        if (random <= cumulative_probability)
        {
            return i;
        }
    }
    // If no node is selected, return -1
    return -1;
}

void construct_path(int start_point, int end_point)
{
    int ants[MAX_ANTS][MAX_CITIES];
    for (int i = 0; i < MAX_ANTS; i++)
    {
        ants[i][0] = start_point;
        for (int j = 1; j < MAX_CITIES - 1; j++)
        {
            int current_node = ants[i][j - 1];
            int next_node = select_next_node(current_node, end_point);
            if (next_node == -1)
            {
                break; // No valid next node found
            }
            ants[i][j] = next_node;
            if (next_node == end_point)
            {
                break; // Reached the end point
            }
        }
        ants[i][MAX_CITIES - 1] = end_point; // Set the ending point for each ant
    }
}

void update_pheromone(int start_point, int end_point)
{
    // Update the pheromone levels on each edge based on the quality of the solutions found by the ants
    double delta_pheromone[MAX_CITIES][MAX_CITIES] = {0.0};
    for (int i = 0; i < MAX_ANTS; i++)
    {
        double tour_length = 0.0;
        for (int j = 0; j < MAX_CITIES - 1; j++)
        {
            int from = ants[i][j];
            int to = ants[i][j + 1];
            tour_length += distance[from][to];
        }
        if (ants[i][MAX_CITIES - 1] == end_point)
        {
            int from = ants[i][MAX_CITIES - 1];
            int to = ants[i][0];
            tour_length += distance[from][to];
        }
        for (int j = 0; j < MAX_CITIES - 1; j++)
        {
            int from = ants[i][j];
            int to = ants[i][j + 1];
            delta_pheromone[from][to] += QVAL / tour_length;
            delta_pheromone[to][from] = delta_pheromone[from][to];
        }
        if (ants[i][MAX_CITIES - 1] == end_point)
        {
            int from = ants[i][MAX_CITIES - 1];
            int to = ants[i][0];
            delta_pheromone[from][to] += QVAL / tour_length;
            delta_pheromone[to][from] = delta_pheromone[from][to];
        }
    }
    for (int i = 0; i < MAX_CITIES; i++)
    {
        for (int j = 0; j < MAX_CITIES; j++)
        {
            pheromone[i][j] = pheromone[i][j] * (1.0 - RHO) + delta_pheromone[i][j];
        }
    }
}

double calculate_tour_length(int ant_path[])
{
    double tour_length = 0.0;
    for (int j = 0; j < MAX_CITIES - 1; j++)
    {
        int from = ant_path[j];
        int to = ant_path[j + 1];
        tour_length += distance[from][to];
    }
    // Add the distance from the last city back to the starting point
    tour_length += distance[ant_path[MAX_CITIES - 1]][ant_path[0]];
    return tour_length;
}

void ant_tour()
{
    for (int i = 0; i < MAX_ANTS; i++)
    {
        length[i] = calculate_tour_length(ants[i]);
    }
}

void print_best_solution()
{
    double best_length = length[0];
    int best_ant_index = 0;
    for (int i = 1; i < MAX_ANTS; i++)
    {
        if (length[i] < best_length)
        {
            best_length = length[i];
            best_ant_index = i;
        }
    }
    printf("Best path: ");
    for (int j = 0; j < MAX_CITIES; j++)
    {
        printf("%d ", ants[best_ant_index][j]);
    }
    printf("\nBest length: %f\n", best_length);
}

int main()
{
    srand(time(NULL));
    int start_point = 0;
    int end_point = 10;
    antancs *graph = (antancs *)malloc(sizeof(antancs));
    graph->V = MAX_CITIES;
    graph->adjacency_list = (node **)malloc(graph->V * sizeof(node *));
    for (int i = 0; i < graph->V; i++)
    {
        graph->adjacency_list[i] = (node *)malloc(graph->V * sizeof(node));
    }
    FILE *file = fopen("newer.csv", "r");
    if (file == NULL)
    {
        perror("Error opening the file");
        return 1;
    }
    putdata(graph, file);
    for (int i = 0; i < graph->V; i++)
    {
        for (int j = 0; j < graph->V; j++)
        {
            distance[i][j] = graph->adjacency_list[i][j].weight;
        }
    }
    initialize_pheromone();
    for (int i = 0; i < 100; i++)
    {
        generate_ants(start_point, end_point);
        construct_path(start_point, end_point);
        update_pheromone(start_point, end_point);
    }
    for (int i = 0; i < 100; i++)
    {
        construct_path(start_point, end_point);
        update_pheromone(start_point, end_point);
        ant_tour();
    }
    print_best_solution();
    return 0;
}