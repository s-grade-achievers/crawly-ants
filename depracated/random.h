#ifndef RANDOM_H
#define RANDOM_H

typedef struct Randoms
{
    long xpto;
} Randoms;

void initRandoms(Randoms *r, long x);
float ran1(long *idum);
float gaussdev(long *idum);
float Normal(Randoms *r, double avg, double sigma);
float Uniforme(Randoms *r);
float sorte(Randoms *r, int m);

#endif
