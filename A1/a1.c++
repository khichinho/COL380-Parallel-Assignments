#include <stdio.h>
#include <stdlib.h>

#include <malloc.h>

int n; // Matrix Size
char matrix_file;
double *matrix_a;

// void read_matrix(const char* matrix_filename, unsigned long* N, ){
//     FILE *fin = fopen(dataset_filename, "r");

//     *matrix = (double*) malloc( sizeof(double) * (*N) * (*N));

//     for(int i=0; i< (*N)*(*N); i++){
//             fscanf(fin, "%d", (*matrix + i));
//         }
//     }
//     fclose(fin)
// }


int main(int argc, char *argv[])
{
    if(argc != 3)
    {
        printf("Enter the size (N) of matrix (NxN) to be input: ");
        scanf("%d",n);

        printf("Enter file containing input matrix(a): ");
        scanf("%s", matrix_file);
    }
    else{
        n=atol(argv[1]);
        matrix_file=(argv[2]);
    }

    // read_matrix(matrix_file, &n, &data)
    return(0);
}