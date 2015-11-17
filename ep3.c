#include <math.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#define E 0.0001 /*error*/

void vector_times_matrix(double *v, double **A, int size, int k) {
	double *aux;
	int size_on_k, i, j;
	size_on_k = size - k; /*size of vector u on step k */
	aux = (double*) malloc(size_on_k*sizeof(double));
	for (j = 0; j < size_on_k; j ++)
		aux[j] = 0;

	for (i = k; i < size; i ++)
		for (j = k + 1; j < size; j ++)
			aux[j - k - 1] += v[i]*A[i][j];

	for (j = 0; j < size_on_k; j ++)
		v[j + k] = aux[j];

	free(aux);
}

void update_matrix(double *v, double **A, double *gamma, int size, int k) {
	int i, j;

	for (i = k + 1; i < size; i ++)
		v[i] = A[i][k]*gamma[k];

	vector_times_matrix(v, A, size, k);

	for (i = k; i < size; i ++)
		for (j = k + 1; j < size; j ++)
			A[i][j] -= A[i][k]*v[j];
}

double **alloc_matrix(int rows, int cols) {
    double **mat = (double **) malloc(sizeof(double *)*rows);
    int i;
    for(i = 0; i < rows; i ++)
        mat[i] = (double *) malloc(sizeof(double)*cols);
    return mat;
} 

void print_matrix(int rows, int columns, double **A){
  int i = 0,j = 0;

  for(i = 0; i < rows; i ++){    /* Iterate of each row */
        for(j = 0; j < columns; j ++){  /* In each row, go over each col element  */
            printf("%f ", A[i][j]); /* Print each row element */
        }
        printf("\n");
    }
}

void free_matrix(int rows, double **mat){
    int i=0;
    for(i=0;i<rows;i++)    
        free(mat[i]);
    free(mat);
}


double QR_decomposition(int n, double **A, int k, double *gamma) {
	double max, norm2;
	int i;
	max = 0;
	norm2 = 0;

	for (i = 0; i < n; i ++)
		if (A[i][k] > max)
			max = A[i][k];

	if (max < E && max > -E) {
		gamma[k] = 0;
		return -1;
	}
	else {		
		for (i = 0; i < n; i ++)
			A[i][k] = A[i][k]/max;
			norm2 += pow(A[i][k], 2);
		norm2 = sqrt(norm2);
		if(A[k][k] < 0)
			norm2 = -norm2;
		A[k][k] = A[k][k] + norm2;
		gamma[k] = 1/(norm2 * A[k][k]);
		A[k][k] = 1;
		return (norm2 * max);
	}
}


/* ****************************************************************************** */
int main() {
	char file_name[100];
	FILE *file;
	double **A, *b, *gamma, duration;
	int n, m, i, j, k;
	clock_t start, end;

	printf("Nome do Arquivo: ");
	scanf("%s", file_name);
	file = fopen(file_name, "r");
	if (file == NULL) {
		fprintf(stderr, "Não foi possível abrir o arquivo!\n");
		return -1;
	}

	fscanf(file, "%d", &n);
	fscanf(file, "%d", &m);
	A = alloc_matrix(n, m);
	b = malloc(n * sizeof(double));
	gamma = malloc(n * sizeof(double));


	for (k = 0; k < n*n; k ++) {
		fscanf(file, "%d %d", &i, &j);
		fscanf(file, "%lf", &A[i][j]);
	}
	for (k= 0; k < n; k ++) {
		fscanf(file, "%d", &i);
		fscanf(file, "%lf", &b[i]);
	}
	/* TESTE */
	start = clock();
	end = clock();
	duration = (double)(end - start) / CLOCKS_PER_SEC;

	free(b);
	free(gamma);
	free_matrix(n, A);

	return 0;
}