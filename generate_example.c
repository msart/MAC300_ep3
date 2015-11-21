#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#define E 0.0001



double **Alocar_matriz_real (int m, int n) {
	double **v; /* ponteiro para a matriz */
	int i; /* variavel auxiliar */
	if (m < 1 || n < 1) { /* verifica parametros recebidos */
		printf ("** Erro: Parametro invalido **\n");
		return (NULL);
	}
	/* aloca as linhas da matriz */
	v = (double **) calloc (m, sizeof(double *));
	if (v == NULL) {
		printf ("** Erro: Memoria Insuficiente **");
		return (NULL);
	}

	/* aloca as colunas da matriz */
	for ( i = 0; i < m; i++ ) {
		v[i] = (double*) calloc (n, sizeof(double));
		if (v[i] == NULL) {
			printf ("** Erro: Memoria Insuficiente **");
			return (NULL);
		}
	}
	return (v); /* retorna o ponteiro para a matriz */
}


void Liberar_matriz_real(int m,int n,double **v) {
	int i; /* variavel auxiliar */
	if (v == NULL)
		return ;
	if(m < 1 || n < 1) {/* verifica parametros recebidos */
		printf ("** Erro: Parametro invalido **\n");
		return ;
	}
	for(i = 0; i < m; i++) free (v[i]);/* libera as linhas da matriz */
	free (v);/* libera a matriz */
}

int main() {
	char file_name[50];
	FILE *file;
	double **A, *x, *b;
	int i, j, n, m, y;

	srand((unsigned) time(NULL));
	printf("Entre com n e m: \n");
	scanf("%d %d", &n, &m);
	sprintf(file_name, "matrix.txt");
	file = fopen(file_name, "w" );


	A = Alocar_matriz_real(n, n);
	b = malloc(n*sizeof(double));
	x = malloc(n*sizeof(double));

	printf("Entre com os m coeficientes desejados: \n");
	for (i = 0; i < m; i ++) {
		scanf("%lf", &x[i]);
	}

	for (i = 0; i < n; i ++) {
		b[i] = 0;
		y = rand() % 10;
		printf("%d\n", y);
		for(j = 0; j < m; j ++) {
			A[i][j] = x[j] * pow(y, m - j - 1);
			printf("%f\n", A[i][j]);
			b[i] += A[i][j];
			printf("%f\n\n", b[i]);
		}
	}


 	fprintf(file,"%d %d", n,  m);

  	for (i=0; i<n; i++)
    	for (j=0; j<m; j++)
      		fprintf(file,"\n%3d %3d %f",i,j,A[i][j]);

  	for (i=0; i<n; i++)
      	fprintf(file,"\n%3d %f",i,b[i]);

    Liberar_matriz_real(n,n,A);
    free(b);
    free(x);
  	fclose(file);
	return 0;
}