#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <time.h>
#include <omp.h>
#include <math.h>
#define VERBOSE 0
#define THREADS 4

static int CmpInt(const void *a, const void *b)
{
	return (*(int*)a - *(int*)b);
}

#define troca( A, B) { int t = A; A = B; B = t; } 
void quicksort(int v[], int p, int r) {
	if (p < r) {
		int i = p - 1, j = r, c = v[r];
		while (1) {
			while (v[++i] < c);
			while (c < v[--j]) if (j == p) break;
			if (i > j) break;
			troca(v[i], v[j]);
		}
		troca(v[i], v[r]);
		quicksort(v, p, j);
		quicksort(v, i + 1, r);
	}
}

/* Merge sorted lists A and B into list A.  A must have dim >= m+n */
void merge(int A[], int B[], int m, int n)
{
	int i = 0, j = 0, k = 0;
	int size = m + n;
	int *C = (int *)malloc(size*sizeof(int));
	while (i < m && j < n) {
		if (A[i] <= B[j]) C[k] = A[i++];
		else C[k] = B[j++];
		k++;
	}
	if (i < m) for (int p = i; p < m; p++, k++) C[k] = A[p];
	else for (int p = j; p < n; p++, k++) C[k] = B[p];
	for (i = 0; i < size; i++) A[i] = C[i];
	free(C);
}

/* Merges N sorted sub-sections of array a into final, fully sorted array a */
void arraymerge(int *a, int size, int *index, int N)
{
	int i;
	while (N > 1) {
		for (i = 0; i < N; i++) index[i] = i*size / N; index[N] = size;
#pragma omp parallel for private(i) 
		for (i = 0; i < N; i += 2) {
			if (VERBOSE) fprintf(stderr, "merging %d and %d, index %d and %d (up to %d)\n", i, i + 1, index[i], index[i + 1], index[i + 2]);
			merge(a + index[i], a + index[i + 1], index[i + 1] - index[i], index[i + 2] - index[i + 1]);
			if (VERBOSE) for (int i = 0; i < size; i++) fprintf(stderr, "after: %d %d\n", i, a[i]);
		}
		N /= 2;
	}
}

int quickSortParalelo(int tam, int * vetorparalelo)
{
	/*PARALELO*/
	int threads = THREADS;
	int *index = (int *)malloc((threads + 1)*sizeof(int));
	for (int i = 0; i < threads; i++)
		index[i] = i*tam / threads;
	index[threads] = tam;

	int i;
#pragma omp parallel for private(i)
	for (i = 0; i < threads; i++)
		qsort(vetorparalelo + index[i], index[i + 1] - index[i], sizeof(int), CmpInt);

	/* Merge sorted array pieces */
	if (threads>1) arraymerge(vetorparalelo, tam, index, threads);

}

void adiciona(int valor, int tam, int ** vetor){
	int * novoVetor = malloc(tam * sizeof(int));
	for (int i = 0; i < tam - 1; i++)
		novoVetor[i] = (*vetor)[i];
	novoVetor[tam - 1] = valor;
	int * temp = *vetor;
	*vetor = novoVetor;
	free(temp);
}

int ordenaPorEliminacao(int tam, int ** vetor){
	int aComparar = -1;
	int * novoVetor = malloc(sizeof(int));
	int contador = 0;
	for (int i = 0; i < tam; i++)
	{
		if ((*vetor)[i] > aComparar){
			aComparar = (*vetor)[i];
			contador++;
			adiciona(aComparar, contador, &novoVetor);
		}
	}
	int * temp = *vetor;
	*vetor = novoVetor;
	free(temp);
	return contador;
}

int ordenaPorEliminacaoParalela(int tam, int ** vetor){
	int * novoVetor = malloc(sizeof(int));
	int contador = 0;
	int i;
#pragma omp parallel
	{
		int aComparar = -1;
#pragma omp for 
		for (i = 0; i < tam; i++){
			if (aComparar == -1)
				aComparar = (*vetor)[i];
			if (aComparar >(*vetor)[i])
			{
				aComparar = (*vetor)[i];
#pragma omp critical 
				{
					contador++;
					adiciona(aComparar, contador, &novoVetor);					
				}
			}
		}
	}

	 int resultado = ordenaPorEliminacao(contador, &novoVetor);

	int * temp = *vetor;
	*vetor = novoVetor;
	free(temp);
	return resultado;
}

int ordena(int tam, int tipo, int ** vetor){
	int* vetorParalelo = malloc(tam * sizeof(int));
	for (int i = 0; i < tam; i++)
		vetorParalelo[i] = (*vetor)[i];
	char filename[100];
	sprintf(filename, "resultado_tam%i_tipo%i.txt", tam, tipo);

	FILE * resultado = fopen(filename, "w");

	if (tipo == 1){
			{
				double start = omp_get_wtime();
				quickSortParalelo(tam, vetorParalelo);
				double end = omp_get_wtime();
				fprintf(resultado, "\n\t tempo qs em paralelo %g", end - start);
			}
		{
			double start = omp_get_wtime();
			quicksort((*vetor), 0, tam - 1);
			double end = omp_get_wtime();
			fprintf(resultado, "\n\t tempo qs em serial %g", end - start);
		}
		fclose(resultado);
		return tam;
	}
	else {
		int retorno;
		{
			double start = omp_get_wtime();
			retorno = ordenaPorEliminacao(tam, vetor);
			double end = omp_get_wtime();
			fprintf(resultado, "\n\t tempo elimin em serial %g", end - start);
		}
			{
				double start = omp_get_wtime();
				retorno = ordenaPorEliminacaoParalela(tam, &vetorParalelo);
				double end = omp_get_wtime();
				fprintf(resultado, "\n\t tempo elimin em paralelo %g", end - start);
			}

		fclose(resultado);
		return retorno;
	}
}


int main()
{
	SYSTEM_INFO sysinfo;
	GetSystemInfo(&sysinfo);

	omp_set_num_threads(THREADS);

	printf("Number Of Processors %d \n", sysinfo.dwNumberOfProcessors, sysinfo.dwProcessorType, sysinfo.wProcessorArchitecture);

	char * directoryName = "Dados_Ep2";
	dirent * dp;
	DIR * dir = opendir(directoryName);
	while (dir){
		errno = 0;
		if ((dp = readdir(dir)) != NULL){
			if (dp->d_namlen > 2){
				char * fileName = dp->d_name;
				char fileToOpen[80];
				sprintf(fileToOpen, "%s//%s", directoryName, fileName);

				char fileLenght[80];
				sprintf(fileLenght, "%.*s", dp->d_namlen - 3 - 4, &fileName[3]);
				int fileLenghtI;
				sscanf(fileLenght, "%d", &fileLenghtI);
				printf("\n %s ", dp->d_name);

				/*char * word;
				scanf("%s", &word);*/

				FILE* file = fopen(fileToOpen, "r");
				if (file != NULL)
				{
					int * qs = malloc(fileLenghtI * sizeof(int));
					int * eliminacao = malloc(fileLenghtI * sizeof(int));
					int cur;
					int i = 0;
					do {
						cur = fgetc(file);
						qs[i] = eliminacao[i] = cur;
						i++;
					} while (i < fileLenghtI);
					fclose(file);
					/*
										char * word;
										scanf("%s", &word);*/

					int tam = ordena(fileLenghtI, 1, &qs);
					tam = ordena(tam, 2, &eliminacao);

					if (eliminacao){
						free(eliminacao);
						eliminacao = NULL;
					}

					if (qs){
						free(qs);
						qs = NULL;
					}
				}
				else
					printf("\nArquivo não encontrado");

				/**/
			}
		}
		else
		{
			closedir(dir);
			break;
		}
	}


	printf("Works!");
	char * word;
	scanf("%s", &word);
	return EXIT_SUCCESS;
}

