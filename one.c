#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <math.h>
#include "random.h"

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
    int BESTLENGTH;
    int *BESTROUTE;
} ACO;

ACO *connectcities(char x[], char y[], ACO *aco, Randoms *randoms)
{
    int a, b;
    if (x[0] == '0')
        a = (int)x[1] - 48;
    else
        a = atoi(x);
    if (y[0] == '0') 
        b = (int)y[1] - 48;
    else
        b = atoi(y);
    // printf("%s %s %d %d\n", x, y, a, b);

    if (aco->GRAPH[a][b] != 1)
    {
        aco->GRAPH[a][b] = 1;
        aco->GRAPH[b][a] = 1;
        aco->PHEROMONES[a][b] = Uniforme(randoms) * aco->TAUMAX;
        aco->PHEROMONES[b][a] = aco->PHEROMONES[a][b];
    }
    return aco;
}

antancs *putdata(antancs *graph, FILE *fp, ACO *aco, Randoms *r)
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
            connectcities(src1, dest1, aco, r);
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

ACO *initACO(ACO *aco, int nAnts, int nCities, double alpha, double beta, double q, double ro, double taumax, int initCity)
{
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
            for (int j = 0; j < 2; j++)
            {
                aco->CITIES[i][j] = -1.0;
                aco->PROBS[i][j] = -1.0;
            }
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

ACO *freeACO(ACO *aco)
{
    for (int i = 0; i < aco->NUMBEROFCITIES; i++)
    {
        free(aco->GRAPH[i]);
        free(aco->CITIES[i]);
        free(aco->PHEROMONES[i]);
        free(aco->DELTAPHEROMONES[i]);
        if (i < aco->NUMBEROFCITIES - 1)
        {
            free(aco->PROBS[i]);
        }
    }
    free(aco->GRAPH);
    free(aco->CITIES);
    free(aco->PHEROMONES);
    free(aco->DELTAPHEROMONES);
    free(aco->PROBS);

    for (int i = 0; i < aco->NUMBEROFANTS; i++)
    {
        free(aco->ROUTES[i]);
    }
    free(aco->ROUTES);
    free(aco->BESTROUTE);
    free(aco);
    return NULL;
}

double distance(ACO *aco, int cityi, int cityj)
{
    return sqrt(pow(aco->CITIES[cityi][0] - aco->CITIES[cityj][0], 2) +
                pow(aco->CITIES[cityi][1] - aco->CITIES[cityj][1], 2));
}

int exists(ACO *aco, int cityi, int cityc)
{
    return (aco->GRAPH[cityi][cityc] == 1);
}

int visited(ACO *aco, int antk, int c)
{
    for (int l = 0; l < aco->NUMBEROFCITIES; l++)
    {
        if (aco->ROUTES[antk][l] == -1)
        {
            break;
        }
        if (aco->ROUTES[antk][l] == c)
        {
            return 1;
        }
    }
    return 0;
}

double PHI(ACO *aco, int cityi, int cityj, int antk)
{
    double ETAij = pow(1 / distance(aco, cityi, cityj), aco->BETA);
    double TAUij = pow(aco->PHEROMONES[cityi][cityj], aco->ALPHA);

    double sum = 0.0;
    for (int c = 0; c < aco->NUMBEROFCITIES; c++)
    {
        if (exists(aco, cityi, c))
        {
            if (!visited(aco, antk, c))
            {
                double ETA = pow(1 / distance(aco, cityi, c), aco->BETA);
                double TAU = pow(aco->PHEROMONES[cityi][c], aco->ALPHA);
                sum += ETA * TAU;
            }
        }
    }
    return (ETAij * TAUij) / sum;
}

double length(ACO *aco, int antk)
{
    double sum = 0.0;
    for (int j = 0; j < aco->NUMBEROFCITIES - 1; j++)
    {
        sum += distance(aco, aco->ROUTES[antk][j], aco->ROUTES[antk][j + 1]);
    }
    return sum;
}

double randomno()
{
    int random_integer = rand();
    double random_double = (double)random_integer / RAND_MAX;
    return random_double;
}

int city(ACO *aco, Randoms *r)
{
    double xi = Uniforme(r);
    int i = 0;
    double sum = aco->PROBS[i][0];
    while (sum < xi)
    {
        i++;
        sum += aco->PROBS[i][0];
    }
    return (int)aco->PROBS[i][1];
}

void route(ACO *aco, int antk, Randoms *r)
{
    aco->ROUTES[antk][0] = aco->INITIALCITY;
    for (int i = 0; i < aco->NUMBEROFCITIES - 1; i++)
    {
        int cityi = aco->ROUTES[antk][i];
        int count = 0;
        for (int c = 0; c < aco->NUMBEROFCITIES; c++)
        {
            if (cityi == c)
            {
                continue;
            }
            if (exists(aco, cityi, c))
            {
                if (!visited(aco, antk, c))
                {
                    aco->PROBS[count][0] = PHI(aco, cityi, c, antk);
                    aco->PROBS[count][1] = (double)c;
                    count++;
                }
            }
        }

        if (count == 0)
        {
            return;
        }

        aco->ROUTES[antk][i + 1] = city(aco, r);
    }
}

int valid(ACO *aco, int antk, int iteration)
{
    for (int i = 0; i < aco->NUMBEROFCITIES - 1; i++)
    {
        int cityi = aco->ROUTES[antk][i];
        int cityj = aco->ROUTES[antk][i + 1];
        if (cityi < 0 || cityj < 0)
        {
            printf("%d %d", cityi, cityj);
            return -1;
        }
        if (!exists(aco, cityi, cityj))
        {
            return -2;
        }
        for (int j = 0; j < i - 1; j++)
        {
            if (aco->ROUTES[antk][i] == aco->ROUTES[antk][j])
            {
                return -3;
            }
        }
    }

    if (!exists(aco, aco->INITIALCITY, aco->ROUTES[antk][aco->NUMBEROFCITIES - 1]))
    {
        return -4;
    }

    return 0;
}

void updatePHEROMONES(ACO *aco)
{
    for (int k = 0; k < aco->NUMBEROFANTS; k++)
    {
        double rlength = length(aco, k);
        for (int r = 0; r < aco->NUMBEROFCITIES - 1; r++)
        {
            int cityi = aco->ROUTES[k][r];
            int cityj = aco->ROUTES[k][r + 1];
            aco->DELTAPHEROMONES[cityi][cityj] += aco->Q / rlength;
            aco->DELTAPHEROMONES[cityj][cityi] += aco->Q / rlength;
        }
    }
    for (int i = 0; i < aco->NUMBEROFCITIES; i++)
    {
        for (int j = 0; j < aco->NUMBEROFCITIES; j++)
        {
            aco->PHEROMONES[i][j] = (1 - aco->RO) * aco->PHEROMONES[i][j] + aco->DELTAPHEROMONES[i][j];
            aco->DELTAPHEROMONES[i][j] = 0.0;
        }
    }
}

void optimize(ACO *aco, int ITERATIONS, Randoms *r)
{
    for (int iterations = 1; iterations <= ITERATIONS; iterations++)
    {
        printf("ITERATION %d HAS STARTED!\n\n", iterations);

        for (int k = 0; k < aco->NUMBEROFANTS; k++)
        {
            printf(": ant %d has been released!\n", k);
            while (0 != valid(aco, k, iterations))
            {
                // printf("%d\n", valid(aco, k, iterations));
                printf(":: releasing ant %d again!\n", k);
                for (int i = 0; i < aco->NUMBEROFCITIES; i++)
                {
                    aco->ROUTES[k][i] = -1;
                }
                route(aco, k, r);
            }

            for (int i = 0; i < aco->NUMBEROFCITIES; i++)
            {
                printf("%d ", aco->ROUTES[k][i]);
            }
            printf("\n");

            printf(":: route done\n");
            double rlength = length(aco, k);

            if (rlength < aco->BESTLENGTH)
            {
                aco->BESTLENGTH = rlength;
                for (int i = 0; i < aco->NUMBEROFCITIES; i++)
                {
                    aco->BESTROUTE[i] = aco->ROUTES[k][i];
                }
            }
            printf(": ant %d has ended!\n", k);
        }

        printf("\nupdating PHEROMONES ...");
        updatePHEROMONES(aco);
        printf(" done!\n\n");

        for (int i = 0; i < aco->NUMBEROFANTS; i++)
        {
            for (int j = 0; j < aco->NUMBEROFCITIES; j++)
            {
                aco->ROUTES[i][j] = -1;
            }
        }

        printf("ITERATION %d HAS ENDED!\n\n", iterations);
    }
}

int main()
{
    Randoms r;
    initRandoms(&r, 42);

    ACO *aco = (ACO *)malloc(sizeof(ACO));
    aco = initACO(aco, 43, 43, 0.5, 2.0, 1.0, 1.0, 1.0, 3);
    FILE *file = fopen("newer.csv", "r");
    if (file == NULL)
    {
        typedef struct Randoms
        {
            long xpto;
        } Randoms;
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
    putdata(graph, file, aco, &r);
    // printf("%f %f %f", aco->PHEROMONES[0][3], aco->PHEROMONES[0][1], graph->adjacency_list[0][0].weight);
    int ITERATIONS = 10; // Change this to the desired number of iterations

    optimize(aco, ITERATIONS, &r);

    aco = freeACO(aco);
    return 0;
}