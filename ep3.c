#include <math.h>
#include <stdio.h>
#include <time.h>
#define E 0.0001 /*erro*/

/*
int forwrow(int n, double A[][nmax], double b[]) {
	int i, j;
	for (i = 0; i < n; i ++) {
		if (A[i][i] < E && A[i][i] > -E)
			return -1;
		for (j = 0; j < i; j ++)
			b[i] -= A[i][j]*b[j];
		b[i] = b[i]/A[i][i];
	}
	return 0;
}
*/

void vector_times_matrix(double *v, double **A, int size, int k) {
	double *aux;
	size_on_k = size - k; /*size of vector u on step k */
	aux = (double*) malloc(size_on_k*sizeof(double));
	for (j = 0; j < size_on_k; j ++)
		aux[i] = 0;

	for (i = k; i < size; i ++)
		for (j = k + 1; j < size; j ++)
			aux[j - k - 1] += v[i]*A[i][j];

	for (j = 0; j < size_on_k; j ++)
		v[j + k] = aux[j];

	free(aux);
}

void update_matrix(double *v, double **A, double *y, int size, int k) {
	int i;

	for (i = k + 1; i < size; i ++)
		v[i] = A[i][k]*y[k];

	vector_times_matrix(v, A, size, k);

	for (i = k; i < size; i ++)
		A[i][j] -= A[i][k]*v[j]
		for (j = k + 1; j < size; j ++)
			A[i][j] -= A[i][k]*v[j]
}

double **alloc_matrix(int rows, int cols) {
    double **mat = (double **) malloc(sizeof(double *)*rows);
    for(i=0; i<rows; i++)
        mat[i] = (double *) malloc(sizeof(double)*cols);
    return mat;
} 

void print_matrix(int rows, int cols, double **mat){
    int i=0,j=0;
  for(i=0; i<rows; i++){    /* Iterate of each row */
        for(j=0; j<cols; j++){  /* In each row, go over each col element  */
            printf("%lf ",mat[i][j]); /* Print each row element */
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


double QR_decomposition(int n, double **A, int k, double *gama) {
	double max, norm2;
	int j;
	max = 0;
	norm2 = 0;

	for (i = 0; i < n; i ++)
		if (A[i][k] > max)
			max = A[i][k];

	if (max == 0)
		gama[k] = 0;
	else {		
		for (i = 0; i < n; i ++)
			A[i][k] = A[i][k]/max;
			norm2 += pow(A[i][k], 2);
		norm2 = sqrt(norm2);
		if(A[k][k] < 0)
			norm2 = -norm2;
		A[k][k] = A[k][k] + norm2;
		gama[k] = 1/(norm2 * A[k][k]);
		A[k][k] = 1
		return (norm2 * max);
	}
}


/* ****************************************************************************** */
int main() {
	char file_name[100];
	FILE *file;
	double **A, *b, *gama, duration;
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
	gama = malloc(n * sizeof(double));


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
	free(gama);
	free_matrix(n, A);

	return 0;
}
