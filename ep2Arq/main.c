#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <time.h>
//
//int separa(int v[], int p, int r)
//{
//	int c = v[p], i = p + 1, j = r, t;
//	while (1) {
//		while (i <= r && v[i] <= c) ++i;
//		while (c < v[j]) --j;
//		if (i >= j) break;
//		t = v[i], v[i] = v[j], v[j] = t;
//		++i; --j;
//	}
//	v[p] = v[j], v[j] = c;
//	return j;
//}
//
//void quicksort(int vetor[], int start, int end)
//{	
//	int j; 
//	if (start < end){
//		j = separa(vetor, start, end);	
//		quicksort(vetor, start, j-1);
//		quicksort(vetor, j + 1, end);
//	} 
//}

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

int ordena(int tam, int tipo, int* vetor){
	if (tipo == 1){
		clock_t startclock = clock();
		quicksort(vetor, 0, tam - 1);
		double d = clock() - startclock, msec;
		msec = d;
		printf("Ordenação %i demorou=%f milissegundos", tam, msec);
		return tam;
	}
	else {
		return sizeof(vetor) / sizeof(int);
	}
}


int main()
{
	char * directoryName = "Dados_Ep2";
	dirent * dp;
	DIR * dir = opendir(directoryName);
	while (dir){
		errno = 0;
		if ((dp = readdir(dir)) != NULL){
			if (dp->d_namlen > 2){
				char * fileName = dp->d_name;
				char fileToOpen[80];
				sprintf(fileToOpen,"%s//%s", directoryName, fileName);

				char fileLenght[80];
				sprintf(fileLenght, "%.*s", dp->d_namlen - 3 -4, &fileName[3]);
				int fileLenghtI;
				sscanf(fileLenght, "%d", &fileLenghtI);
				printf("\nArquivo a processar: %s, numeros: %s \n",fileToOpen, fileLenght);
				
				/*char * word;
				scanf("%s", &word);*/

				FILE* file = fopen(fileToOpen, "r");
				if (file != NULL)
				{
					int * vector = malloc(fileLenghtI * sizeof(int));
					int cur;
					int i = 0;
					do {
						cur = fgetc(file);						
						vector[i] = cur;
						i++;
					} while (i < fileLenghtI);
					fclose(file);					

					//printf("\nVetor inicial: ");
					//for (i = 0; i < fileLenghtI; i++)
					//	printf("%i,", vector[i]);
					//printf("\n\n");
					
					ordena(fileLenghtI, 1, vector);

					//printf("\nVetor processado: ");
					//for (i = 0; i < fileLenghtI; i++)
					//	printf("%i,", vector[i]);
					//printf("\n\n");
					
					//processa.
					free(vector);
					vector = NULL;
				}
				else
					printf("Arquivo não encontrado");
		
				printf("\n\nProcessamento concluído");
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

