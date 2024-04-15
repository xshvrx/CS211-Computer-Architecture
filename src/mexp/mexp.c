#include <stdio.h>
#include <stdlib.h>

int ** createMatrix(int k){
    int ** matrix = (int **)malloc(sizeof(int*) * k);
    int i;
    for( i=0; i<k; i++ ) {
        matrix[i] = (int *)malloc(sizeof(int) * k);
    }
    return matrix;
}

void printMatrix(int ** matrix, int k){
    int i,j;
    for( i=0; i<k; i++ ){
        for( j=0; j<k; j++ ){
            printf("%d", matrix[i][j]);
            if( j<k-1) {
                printf(" ");
            }
        }
        printf("\n");
    }
}

void readMatrix(FILE * fp, int ** matrix, int k){
    int i,j;
    for( i=0; i<k; i++ ){
        for( j=0; j<k; j++ ){
            int val;
            fscanf(fp, "%d", &val);
            matrix[i][j] = val;
        }
    }
}

void copyMatrix(int ** s, int ** d, int k) {
    int i,j;
    for( i=0; i<k; i++ ){
        for( j=0; j<k; j++ ){
            d[i][j] = s[i][j];
        }
    }
}

// C will store the result of A into B
void multiply(int ** A, int ** B, int ** C, int k) {
    int i,j,l;
    for( i=0; i<k; i++ ){
        for( j=0; j<k; j++ ){
            C[i][j] = 0;
            for( l=0; l<k; l++ )    
            {     
                C[i][j] += A[i][l] * B[l][j];    
            } 
        }
    }
}

void freeMatrix(int ** matrix, int k) {
    int i;
    for( i=0; i<k; i++ ) {
        free(matrix[i]);
    }
    free(matrix);
}

int main(int argc, char *argv[]){

    if( argc == 1 ) {
        return 0;
    }

    char * filename = argv[1];
    FILE * fp = fopen(filename, "r");
    if( !fp ) {
        return 0; 
    }
    int k;
    fscanf(fp, "%d", &k);
    int ** matrix = createMatrix(k);
    readMatrix(fp,matrix,k);
    int n;
    fscanf(fp, "%d", &n);
    fclose(fp);

    if( n==0 ){
        int i,j;
        for( i=0; i<k; i++ ){
            for( j=0; j<k; j++ ) {
                matrix[i][j] = 0;
            }
        }
        for( i=0; i<k; i++ ){
            matrix[i][i] = 1;
        }

        printMatrix(matrix,k);
        freeMatrix(matrix,k);
    }
    else {
        int ** A = createMatrix(k);
        copyMatrix(matrix,A,k);
        int ** B = createMatrix(k);
        copyMatrix(matrix,B,k);

        int ** C = createMatrix(k);
        copyMatrix(matrix,C,k);
        for( int i=1; i<n; i++ ){
            multiply(A, B, C, k);

            copyMatrix(C, A, k);
            copyMatrix(matrix, B, k);
        }

        printMatrix(C,k);

        freeMatrix(A,k);
        freeMatrix(B,k);
        freeMatrix(C,k);
        freeMatrix(matrix,k);
    }

    return 0;
}
