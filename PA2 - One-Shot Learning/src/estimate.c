#include <stdio.h>
#include <stdlib.h>
#include <string.h>

double** makeArray(int rows, int columns){
	double **matrix = (double **)malloc(rows*sizeof(double *));
	for (int i=0; i<rows; i++){
		matrix[i] = (double *)malloc(columns*sizeof(double));
	}
	return matrix;
}

void printMatrix(double **matrix, int rows, int columns){
	printf("\n");	
	for (int i=0; i<rows; i++){
		for (int j=0; j<columns; j++){
			printf("%f", matrix[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}

double** transposeMatrix(double** matrix, int rows, int columns){

	double **result = makeArray(columns, rows);
	for (int i=0; i<columns; i++){
		for (int j=0; j<rows; j++){
			result[i][j] = matrix[j][i];
		}
	}
	return result;
}

double** multiplyMatrix(double** matA, double** matB, int rows1, int columns1, int rows2, int columns2){

	double **result = makeArray(rows1, columns2);
	for (int i=0; i<rows1; i++){
		for (int j=0; j<columns2; j++){
			result[i][j] = 0;
		}
	}
	for (int i=0; i<rows1; i++){
		for (int j=0; j<columns2; j++){
			for (int k=0; k<rows2; k++){
				result[i][j] += matA[i][k]*matB[k][j];
			}
		}
	}

	return result;
}

double** invertMatrix(double **matrix, int dimension){

	double **result = makeArray(dimension, dimension);
	for(int i = 0; i < dimension; i++){
    		for(int j = 0; j < dimension; j++){
      			if(i!=j){ 
      				result[i][j] = 0;
      			}
      			else {
      				result[i][j] = 1;
      			}
    		}
  	}
	double multiplier;
	double multiplier2;
	for(int k = 0; k < dimension; k++){
    		for(int l = k; l < dimension; l++){
      			if(matrix[l][k] != 1 && k == l){
				if(matrix[l][k] != 0){
	  				multiplier = matrix[l][k];
	  				for(int m = 0; m < dimension; m++){
	    					matrix[l][m] = matrix[l][m]/multiplier;
	    					result[l][m] = result[l][m]/multiplier;
	  				}
				}
      			}
      			else if(k != l && matrix[l][k] != 0) {
				multiplier2 = matrix[l][k];
				for(int m = 0; m < dimension; m++) {
	 				matrix[l][m] = matrix[l][m]- (multiplier2*matrix[k][m]);
			      		result[l][m] = result[l][m] - (multiplier2*result[k][m]);
				}
      			}
    		}
 	}
 	for(int k = dimension-1; k > -1; k--){
 		for(int l = k; l >-1; l--){
      			if(k == l && matrix[l][k] != 1){
				multiplier = matrix[l][k];
				matrix[l][k] = matrix[l][k] / multiplier;
				result[l][k] = result[l][k] / multiplier;
      			}
      			else if (l != k && matrix[l][k] != 0) {
				multiplier = matrix[l][k];
				for(int m = 0; m < dimension; m++){
          				matrix[l][m] = matrix[l][m] - (matrix[k][m]) * multiplier;
	  				result[l][m] = result[l][m] - (result[k][m]) * multiplier;
        			}
      			}
    		}
	}
  	return result;
}

void freeMatrix(double **matrix, int rows){
	for (int i=0; i<rows; i++){
		free(matrix[i]);
	}
	free(matrix);
}

int main(int argc, char **argv) {
	FILE* fptr = fopen(argv[1], "r");
	char matrixType[10];
	int columns;
	int rows;
	fscanf(fptr, "%s", matrixType);
	fscanf(fptr, "%d", &columns);
	fscanf(fptr, "%d", &rows);
	columns++;
	double **matrix;
	matrix = makeArray(rows, columns);
	FILE* fpd = fopen(argv[2], "r");
	int rows2;
	fscanf(fpd, "%s", matrixType);
	fscanf(fpd, "%d", &columns);
	fscanf(fpd, "%d", &rows2);
	columns++;
	double **matrix2;
	matrix2 = makeArray(rows2, columns-1);
	
	if (fptr == NULL || fpd == NULL) {
	printf("Error train/test file not found");
        return -1;
    	}

	for (int i=0; i<rows; i++){
		for (int j=0; j<columns; j++){
			fscanf(fptr, "%lf", &matrix[i][j]);
		}
		fscanf(fptr, "\n");
	}
	fclose(fptr);

	for (int i=0; i<rows2; i++){
		for (int j=0; j<columns-1; j++){
			fscanf(fpd, "%lf", &matrix2[i][j]);
		}
		fscanf(fpd, "\n");
	}
	fclose(fpd);

	double **xMatrix;
	xMatrix = makeArray(rows,columns);
	for (int i=0; i<rows; i++){
		xMatrix[i][0] = 1;
		for (int j=1; j<columns; j++){
			xMatrix[i][j] = matrix[i][j-1];
		}
	}
	double **yMatrix;
	yMatrix = makeArray(rows, 1);
	for (int i=0; i<rows; i++){
		yMatrix[i][0] = matrix[i][columns-1];
	}
	double prediction = 0.0;
	double **transpose = transposeMatrix(xMatrix, rows, columns);
	double **product = multiplyMatrix(transpose, xMatrix, columns, rows, rows, columns);
	double **inverse = invertMatrix(product, columns);
	double **total = multiplyMatrix(inverse, transpose, columns, columns, columns, rows);
	double **w = multiplyMatrix(total, yMatrix, columns, rows, rows, 1);
	for (int i=0; i<rows2; i++){
		for (int j=-1; j<columns-1; j++){
			if (j==-1){	
				prediction += w[0][0];
				continue;
			}
			else{
				prediction += w[j+1][0]*matrix2[i][j];
			}
		}
		printf("%.0f\n", prediction);
		prediction = 0;
	}
	freeMatrix(matrix, rows);
	freeMatrix(matrix2, rows2);
	freeMatrix(xMatrix, rows);
	freeMatrix(yMatrix, rows);
	freeMatrix(transpose, columns);
	freeMatrix(product, columns);
	freeMatrix(inverse, columns);
	freeMatrix(total, columns);
	freeMatrix(w, columns);	
	return 0;
}