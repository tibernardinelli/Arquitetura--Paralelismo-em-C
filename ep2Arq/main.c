#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

#define _CRT_SECURE_NO_WARNINGS

int main()
{
	char * directoryName = "c://ep2//";
	dirent * dp;
	DIR * dir = opendir(directoryName);
	while (dir){
		errno = 0;
		if ((dp = readdir(dir)) != NULL){
			if (dp->d_namlen > 2){
				char * fileName = dp->d_name;
				char fileToOpen[80];
				sprintf(fileToOpen,"%s%s", directoryName, fileName);

				char fileLenght[80];
				sprintf(fileLenght, "%.*s", dp->d_namlen - 3 -4, &fileName[3]);
					
				printf("Arquivo a processar: %s, numeros: %s \n",fileToOpen, fileLenght);
				

				FILE* file = fopen(fileToOpen, "r");
				if (file != NULL)
				{
					
					fclose(file);					
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

int quickSortSerial(int tam, int tipo, int* vetor){

}