#include <math.h>
#include <stdio.h>
#include <time.h>
#define nmax 700
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
void QR_decomposition(int n, double A[][nmax], int i, double gama[]) {
	double max, norm2;
	int j;
	max = 0;
	norm2 = 0;
	for (j = 0; j < n; j++)
		if (A[i][j] > max)
			max = A[i][j];
		
	if (max == 0)
		gama[i] = 0;
	else {		
		for (j = 0; j < n; j++)
			A[i][j] = A[i][j]/max;
		for (j = 0; j < n; j++)
			norm2 = pow(A[i][j], 2);
		norm2 = sqrt(norm2);
		if(A[i][0] > 0)
			norm2 = -norm2;
		A[i][0] = A[i][0] + norm2;
		gama[i] = 1/(norm2 * A[i][0]);
		gama[i] = gama[i] * max;

	}
}


/* ****************************************************************************** */
int main() {
	char file_name[100];
	FILE *file;
	double A[nmax][nmax], b[nmax], duration;
	int n, i, j, k;
	clock_t start, end;

	printf("Nome do Arquivo: ");
	scanf("%s", file_name);
	file = fopen(file_name, "r");
	if (file == NULL) {
		fprintf(stderr, "Não foi possível abrir o arquivo!\n");
		return -1;
	}

	fscanf(file, "%d", &n);
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

	return 0;
}
