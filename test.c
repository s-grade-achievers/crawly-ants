#include <stdio.h>
#include <stdlib.h>

int mn()
{
    int random_integer = rand();                              // Generates a random integer between 0 and RAND_MAX
    double random_double = (double)random_integer / RAND_MAX; // Scales it to a double between 0 and 1

    printf("Random number between 0 and 1: %f\n", random_double);

    return 0;
}

int main()
{
    mn();
    mn();
    mn();
    mn();
}