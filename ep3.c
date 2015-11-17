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
void QR_decomposition(int n, double A[][nmax], double b[]) {  /*pensar como vai ser por linha*/

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
