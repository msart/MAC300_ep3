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
	update_vector(b, QR, rows, columns, gamma);
	backrow(QR, b, columns);
}

double generating_Q(int n, double **A, int k, double *gamma, double *norms) {
	double max, t;
	int i;
	max = 0;

	for (i = k; i < n; i ++)
		if (fabs(A[i][k]) > max)
			max = fabs(A[i][k]);

	if (max < E) {
		gamma[k] = 0;
		return -1;
	}
	else {
		t = 0;	
		for (i = k; i < n; i ++)
			A[i][k] = A[i][k]/max;
		t = norms[k]/max;

		if(A[k][k] < 0)
			t = -t;
		A[k][k] = A[k][k] + t;
		gamma[k] = A[k][k]/(t);
		for (i = k + 1; i < n; i ++) {
			A[i][k] = A[i][k]/A[k][k];
		}
		A[k][k] = 1;
		return (t * max);
	}
}

void update_norms_vector(double **A, int rows, int columns, double *norms, int k, double *max) {
	int i, j;
	if (k == 0) {

		for (j = k; j < columns; j ++) {
			norms[j] = max[j] = 0;
		}

		for (i = 0; i < rows; i ++)
			for (j = 0; j < columns; j ++){
				if (fabs(A[i][j]) > max[j])
					max[j] = fabs(A[i][j]);
			}
		
		for (i = 0; i < rows; i ++)
			for (j = 0; j < columns; j ++)
				norms[j] += pow(A[i][j]/max[j], 2);

		for (j = 0; j< columns; j++)
			norms[j] = sqrt(norms[j]) * max[j];
	}
	else {
		for (j = k; j < columns; j ++){
			norms[j] = pow(norms[j]/max[j], 2);
			norms[j] -= pow(A[k - 1][j]/max[j], 2);
			norms[j] = sqrt(norms[j]) * max[j];
		}
	}
}

void permute(double **A, int rows, int columns, int *permutation, double *norms, int k) {
	int i, j, column_with_max_norm;
	double swap;
	column_with_max_norm = k;

	for (j = k + 1; j < columns; j ++)
			if(norms[j] > norms[column_with_max_norm])
				column_with_max_norm = j;

	permutation[k] = column_with_max_norm;	
	
	for(i = 0; i < rows; i++) {
		swap = A[i][k];
		A[i][k] = A[i][column_with_max_norm];
		A[i][column_with_max_norm] = swap;
	}
	swap = norms[k];
	norms[k] = norms[column_with_max_norm];
	norms[column_with_max_norm] = swap;
}

void QR_decomposition(double **A, double *gamma, int rows, int columns, int *permutation) {
	int k;
	double t, *norms, *max;
	norms = malloc(columns * sizeof(double));
	max = malloc(columns * sizeof(double));
	for (k = 0; k < columns; k ++) {
		update_norms_vector(A, rows, columns, norms, k, max);
		permute(A, rows, columns, permutation, norms, k);
		t = generating_Q(rows, A, k, gamma, norms);
		update_matrix(A, gamma, rows, columns, k);
		A[k][k] = -t;
	}
	free(max);
	free(norms);
}

/* ****************************************************************************** */
int main() {
	char file_name[100];
	FILE *file;
	double **A, *b, *gamma, swap;
	int n, m, i, j, k, *permutation;

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
	gamma = malloc(m * sizeof(double));
	permutation = malloc (m * sizeof(int));
	for(k = 0; k < m; k++)
		permutation[k] = k;

	for (k = 0; k < n*m; k ++) {
		fscanf(file, "%d %d", &i, &j);
		fscanf(file, "%lf", &A[i][j]);
	}
	
	for (k= 0; k < n; k ++) {
		fscanf(file, "%d", &i);
		fscanf(file, "%lf", &b[i]);
	}
	QR_decomposition(A, gamma, n, m, permutation);

	solve_QR_system(A, n, m, b, gamma);
	
	for (k = m - 1; k >= 0; k --) {
		swap = b[k];
		b[k] = b[permutation[k]];
		b[permutation[k]] = swap;
	}
	/*vetor de resposta em b*/
	/*IMPORTANTE: so posso fazer isso pois o sistema e sobre-determinado*/
	for (n = 0; n < m; n ++)
		printf("%f ", b[n]);
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
	free(permutation);
	free(b);
	free(gamma);
	free_matrix(n, A);

	return 0;
}
