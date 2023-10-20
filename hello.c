#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(){
    FILE *file = fopen("c:\\Users\\soura\\Downloads\\Ant-colony\\idunno.csv", "r");
    if (file == NULL) {
        perror("Error opening the file");
        return 1;
    }
    

    return 0;
}