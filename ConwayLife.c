#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main (int argc, char* argv[]){
	
	int numGens = atoi (argv[1]);
	int matrixSize = atoi (argv[2]);
	int numThreads = atoi (argv[3]);
	char* filename;
	filename = argv[4];

	FILE* fileptr;
	fileptr = fopen (filename, "r");
	char c = fgetc(fileptr);

	int original [matrixSize][matrixSize];
	int nextGen [matrixSize][matrixSize];
	//File and print original matrix
	for (int a = 0; a < matrixSize; a++){
		for (int b = 0; b < matrixSize; b++){
			original[a][b] = (int) c;
			c = fgetc(fileptr);
			c = fgetc(fileptr);
		}
		c = fgetc(fileptr);
	}

	char cell;
	int aliveNeighbors;
	char tempCell;

	//Timing Program
	double start = omp_get_wtime();
	double finish;
	//Create threads before for loop so they're not destoryed/created every iteration
//	#pragma omp parallel
	for (int d = 0; d < numGens; d++){
		//Parallelize outer looping
//		#pragma omp parllel for
		//Loop through original matrix rows
		for (int e = 0; e < matrixSize; e++){
			//Loop through original matrix columns
			for (int f = 0; f < matrixSize; f++){
				cell = original [e][f];
				//Reset # of alive neighbors
				aliveNeighbors = 0;
				//Evaluating cell's neighbors
				for (int g = e-1; g <= e+1; g++){
					if(g >= 0 && g <= matrixSize-1){
						for(int h = f-1; h <= f+1; h++){
							if(h >= 0 && h <= matrixSize-1){
								if (g != e || h != f){
									tempCell = original [g][h];
									if (tempCell == '1'){
										aliveNeighbors++;
									}
								}
							}
						}
					}
				}
				//Checking next generation conditions
				if (cell == '0'){
					if (aliveNeighbors == 3){
						nextGen[e][f] = '1';
					}
					else{
						nextGen[e][f] = '0';
					}
				}
				else{
					if(aliveNeighbors < 2){
						nextGen[e][f] = '0';
					}
					else if (aliveNeighbors > 3){
						nextGen[e][f] = '0';
					}
					else{
						nextGen[e][f] = '1';
					}
				}
			}
		}
		//Update working matrix to nextGen
		for (int i = 0; i < matrixSize; i++){
			for (int j = 0; j < matrixSize; j++){
				original[i][j] = nextGen[i][j];
			}
		}
	}
	//Print elapsed time
	finish = omp_get_wtime();
	printf("Time:%f\n", finish-start);

	//Write final matrix to file
	fclose(fileptr);
	FILE* fileptr1;
	fileptr1 = fopen("output.txt", "w");
	char m;
	for (int n = 0; n < matrixSize; n++){
		for (int o = 0; o < matrixSize; o++){
			m = original[n][o];
			fprintf(fileptr1, " ");
			fputc(m, fileptr1);
		}
		fprintf (fileptr1, "\n");
	}
	fclose(fileptr1);

}
