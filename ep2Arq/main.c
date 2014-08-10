#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

#define _CRT_SECURE_NO_WARNINGS

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
				
				char * word;
				scanf("%s", &word);

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

					printf("\nVetor inicial: ");
					for (i = 0; i < fileLenghtI; i++)
						printf("%i,", vector[i]);
					printf("\n\n");
					
					
					
					//processa.
					free(vector);
					vector = NULL;
				}
				else
					printf("Arquivo não encontrado");
		
				printf("Processamento concluído");
			}
		}
		
	}
	closedir(dir);

	printf("Works!");
	char * word;
	scanf("%s", &word);
	return EXIT_SUCCESS;
}

int ordena(int tam, int tipo, int* vetor){
	if (tipo == 1){
		quicksort(vetor, 0, tam);
		return tam;
	} 
	else {
		return sizeof(vetor)/sizeof(int);
	}
}