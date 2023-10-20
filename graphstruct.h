typedef struct NODE{
    char src[20];
    char dst[20];
    double weight;
}node; 

typedef struct ANTANCS
{
    int V;
    struct NODE *adjacency_list[50][50]; 
}antancs;




