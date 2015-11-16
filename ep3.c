#include <math.h>
#include <stdio.h>
#include <time.h>
#define nmax 700
#define E 0.0001 /*erro*/

int cholrow(int n, double A[][nmax]) {
	int i, j, k;
	for (j = 0; j < n; j ++) {
		for (k = 0; k < j; k ++)
			A[j][j] -= A[j][k]*A[j][k];
		if (A[j][j] < E) /* <= 0 */
			return -1;
		A[j][j] = sqrt(A[j][j]);
		for (i = j + 1; i < n; i ++) {
			for (k = 0; k < j; k ++) {
				A[i][j] -= A[i][k]*A[j][k];
			}
			A[i][j] = A[i][j]/A[j][j];
		}
	}
	return 0;
}

int forwrow(int n, double A[][nmax], double b[]) {
	int i, j;
	for (i = 0; i < n; i ++) {
		if (A[i][i] < E && A[i][i] > -E) /* == 0 */
			return -1;
		for (j = 0; j < i; j ++)
			b[i] -= A[i][j]*b[j];
		b[i] = b[i]/A[i][i];
	}
	return 0;
}

int backrow(int n, double A[][nmax], double b[], int trans) {
	int i, j;
	/* ESSE IF ESTÁ ERRADO*/
	if (trans) { /* trans == 1 */
		for (j = n - 1; j >= 0; j --) {
			if (A[j][j] < E && A[j][j] > -E) /* == 0 */
				return -1;
			b[j] = b[j]/A[j][j];
			for (i = 0; i < j; i ++)
				b[i] -= A[j][i]*b[j];
		}
	}
	else { /* trans == 0 */
		for (i = n - 1; i >= 0; i --) {
			if (A[i][i] < E && A[i][i] > -E) /* == 0 */
				return -1;
			for (j = i + 1; j < n; j ++)
				b[i] -= A[i][j]*b[j];
			b[i] = b[i]/A[i][i];
		}
	}
	return 0;
}
/* ****************************************************************************** */
int cholcol(int n, double A[][nmax]) {
	int i, j, k;
	for (i = 0; i < n; i ++) {
		if (A[i][i] < E) /* <= 0 */
			return -1;
		A[i][i] = sqrt(A[i][i]);
		for (j = i + 1; j < n; j ++) {
			A[j][i] = A[j][i]/A[i][i];
			A[j][j] -= A[j][i]*A[j][i];
			for (k = i + 1; k < j; k++)
				A[j][k] -= A[j][i]*A[k][i];
		}
	}
	return 0;
}

int forwcol(int n, double A[][nmax], double b[]) {
	int i, j;
	for (j = 0; j < n; j ++) {
		if (A[j][j] < E && A[j][j] > -E) /* == 0 */
			return -1;
		b[j] = b[j]/A[j][j];
		for (i = j + 1; i < n; i ++)
			b[i] -= A[i][j]*b[j];
	}
	return 0;
}

int backcol(int n, double A[][nmax], double b[], int trans) {
	int i, j;
	if (trans) { /* trans == 1 */
		for (i = n - 1; i >= 0; i --) {
			if (A[i][i] < E && A[i][i] > -E) /* == 0 */
				return -1;
			for (j = i + 1; j < n; j ++)
				b[i] -= A[j][i]*b[j]; /* indices da matriz A invertidos */
			b[i] = b[i]/A[i][i];
		}
	}
	else { /* trans == 0 */
		for (j = n - 1; j >= 0; j --) {
			if (A[j][j] < E && A[j][j] > -E) /* == 0 */
				return -1;
			for (i = j + 1; i < n; i ++)
				b[j] -= A[i][j]*b[i];
			b[j] = b[j]/A[j][j];
		}
	}
	return 0;
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
	cholrow(n, A);
	forwrow(n, A, b);
	/*for (i = 0; i < n; i++)
		for (j = 0; j< n; j++)
			A[i][j] = A[j][i];*/
	backrow(n, A, b, 1);
	end = clock();
	duration = (double)(end - start) / CLOCKS_PER_SEC;
	printf("Backrow tempo %e segundos\n", duration);
	for (i = 0; i < n; i ++) {
		if (b[i] - (1 + i%(n/100)) > E || b[i] - (1 + i%(n/100)) < -E)
			printf("Erro! %e  %d %d\n", b[i],-(1 + i%(n/100)), i);
	}
	printf("Fim da Análise!\n");
	return 0;
}
