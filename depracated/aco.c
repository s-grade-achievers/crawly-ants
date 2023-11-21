#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

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

typedef struct
{
    int seed;
} Randoms;

Randoms *initRandoms(int seed)
{
    Randoms *randoms = malloc(sizeof(Randoms));
    randoms->seed = seed;
    return randoms;
}

double Uniforme(Randoms *randoms)
{
    randoms->seed = (randoms->seed * 32719 + 3) % 32749;
    return randoms->seed / 32749.0;
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

void freeACO(ACO *aco)
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
}

void connectCITIES(ACO *aco, int cityi, int cityj, Randoms* randoms)
{
    aco->GRAPH[cityi][cityj] = 1;
    aco->PHEROMONES[cityi][cityj] = Uniforme(randoms) * aco->TAUMAX;
    aco->GRAPH[cityj][cityi] = 1;
    aco->PHEROMONES[cityj][cityi] = aco->PHEROMONES[cityi][cityj];
}

void setCITYPOSITION(ACO *aco, int city, double x, double y)
{
    aco->CITIES[city][0] = x;
    aco->CITIES[city][1] = y;
}

void printPHEROMONES(ACO *aco)
{
    // Implement the printPHEROMONES function
    // ...
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

int vizited(ACO *aco, int antk, int c)
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
            if (!vizited(aco, antk, c))
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

int city(ACO *aco, Randoms *randoms)
{
    double xi = Uniforme(randoms);
    int i = 0;
    double sum = aco->PROBS[i][0];
    while (sum < xi)
    {
        i++;
        sum += aco->PROBS[i][0];
    }
    return (int)aco->PROBS[i][1];
}

void route(ACO *aco, int antk)
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
                if (!vizited(aco, antk, c))
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

        aco->ROUTES[antk][i + 1] = city(aco);
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

void optimize(ACO *aco, int ITERATIONS)
{
    for (int iterations = 1; iterations <= ITERATIONS; iterations++)
    {
        printf("ITERATION %d HAS STARTED!\n\n", iterations);

        for (int k = 0; k < aco->NUMBEROFANTS; k++)
        {
            printf(": ant %d has been released!\n", k);
            while (0 != valid(aco, k, iterations))
            {
                printf(":: releasing ant %d again!\n", k);
                for (int i = 0; i < aco->NUMBEROFCITIES; i++)
                {
                    aco->ROUTES[k][i] = -1;
                }
                route(aco, k);
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

        printPHEROMONES(aco);

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
    ACO *aco = createACO(/* arguments */);
    init(aco);

    int ITERATIONS = 10; // Change this to the desired number of iterations

    optimize(aco, ITERATIONS);

    destroyACO(aco);
    return 0;
}
