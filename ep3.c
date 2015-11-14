#include <math.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#define E 0.0001 /*error*/

double inner_product(double *v1, double *v2, int rows, int k) {
	int i;
	double inner_product = 0;

	for (i = k; i < rows; i ++)
		inner_product += v1[i - k]*v2[i];
	
	return inner_product;
}

int backrow(double **R, double *b, int columns) {
	int i, j;
	for (i = columns - 1; i >= 0; i --) {
		if (fabs(R[i][i]) < E) /* == 0 */
			return -1;
		for (j = i + 1; j < columns; j ++)
			b[i] -= R[i][j]*b[j];
		b[i] = b[i]/R[i][i];
	}
	return 0;
}

void vector_times_matrix(double *v, double **A, int rows, int columns, int k) {
	double *aux;
	int i, j;
	aux = (double*) malloc((columns - k - 1)*sizeof(double));
	for (j = 0; j < (columns - k - 1); j ++)
		aux[j] = 0;

	for (i = k; i < rows; i ++)
		for (j = k + 1; j < columns; j ++)
			aux[j - k - 1] += v[i - k]*A[i][j];

	for (j = 0; j < (columns - k - 1); j ++)
		v[j] = aux[j];                       /* possible as the system is overdetermined */

	free(aux);
}

void update_matrix(double **A, double *gamma, int rows, int columns, int k) {
	int i, j;
	double *v;
	v = malloc((rows - k) * sizeof(double));

	for (i = k; i < rows; i ++)
		v[i - k] = A[i][k]*gamma[k];

	vector_times_matrix(v, A, rows, columns, k);

	for (i = k; i < rows; i ++)
		for (j = k + 1; j < columns; j ++)
			A[i][j] -= A[i][k]*v[j - k - 1];
	free(v);
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
    int i = 0;
    for(i = 0; i < rows; i ++)    
        free(mat[i]);
    free(mat);
}

void update_vector(double *b, double **Q, int rows, int columns, double *gamma) {
	int i, k;
	double *u, factor;
	u = malloc((rows) * sizeof(double));
	for (k = 0; k < columns; k ++) {
		u[0] = 1;
		for (i = k + 1; i < rows; i ++)
			u[i - k] = Q[i][k];
	
		factor = gamma[k]*inner_product(u, b, rows, k);

		for (i = k; i < rows; i ++)
			b[i] -= factor*u[i - k];
	}
	free(u);
}

void solve_QR_system(double **QR, int rows, int columns, double *b, double *gamma) {
	int k;
	print_matrix(rows, columns, QR);
	update_vector(b, QR, rows, columns, gamma);
	for (k = 0; k < rows; k ++)
		printf("%f ", b[k]);
	printf("\n");
	backrow(QR, b, columns);
}

double generating_Q(int n, double **A, int k, double *gamma) {
	double max, norm2;
	int i;
	max = 0;
	norm2 = 0;

	for (i = k; i < n; i ++)
		if (fabs(A[i][k]) > max)
			max = fabs(A[i][k]);

	if (max < E) {
		gamma[k] = 0;
		return -1;
	}
	else {		
		for (i = k; i < n; i ++) {
			A[i][k] = A[i][k]/max;
			norm2 += pow(A[i][k], 2);
		}
		norm2 = sqrt(norm2);
		if(A[k][k] < 0)
			norm2 = -norm2;
		A[k][k] = A[k][k] + norm2;
		gamma[k] = A[k][k]/(norm2);
		for (i = k + 1; i < n; i ++) {
			A[i][k] = A[i][k]/A[k][k];
		}
		A[k][k] = 1;
		return (norm2 * max);
	}
}

void QR_decomposition(double **A, double *gamma, int rows, int columns) {
	int k;
	double t;
	for (k = 0; k < columns; k ++) {
		t = generating_Q(rows, A, k, gamma);
		update_matrix(A, gamma, rows, columns, k);
		A[k][k] = -t;
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
	
	QR_decomposition(A, gamma, n, m);
	solve_QR_system(A, n, m, b, gamma);
	
	for (k = 0; k < n; k ++)
		printf("%f ", b[k]);
	printf("\n");
	/* TESTE */
	/*b[0] = 1;
	for (k = 0; k < n; k ++)
		printf("%f ", b[k]);
	printf("\n");
	print_matrix(n, m, A);
	vector_times_matrix(b, A, n, m, 0);
	for (k = 0; k < m; k ++)
		printf("%f ", b[k]);
	printf("\n");*/
	/*
	start = clock();
	end = clock();
	duration = (double)(end - start) / CLOCKS_PER_SEC;
	*/
	free(b);
	free(gamma);
	free_matrix(n, A);

	return 0;
}
