#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <time.h>

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
		printf(":%f milissegundos", msec);
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
				printf("\n %s ",fileToOpen);
				
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

