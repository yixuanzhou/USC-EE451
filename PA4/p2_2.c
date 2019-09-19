#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>

int main(int argc, char** argv){
    int size, rank;
    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&size);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);

    int numberArray[64];
    int i;
    if (rank == 0){
      FILE *numberFile;
      numberFile = fopen("number.txt", "r");

      for (i = 0; i < 64; i++) {
        fscanf(numberFile, "%d", &numberArray[i]);
      }
    }

    MPI_Bcast(&numberArray, 64, MPI_INT, 0, MPI_COMM_WORLD);

    int localSum;

    if (rank == 0) {
      for (i = 0; i <= 15; i++) {
        localSum += numberArray[i];
      }
    }
    else if (rank == 1){
      for (i = 16; i <= 32; i++) {
        localSum += numberArray[i];
      }
    }
    else if (rank  == 2){
      for (i = 33; i <= 47; i++) {
        localSum += numberArray[i];
      }
    }
    else if( rank  == 3){
      for (i = 48; i <= 63; i++) {
        localSum += numberArray[i];
      }
    }

    int globalSum;
    MPI_Reduce(&localSum, &globalSum, 2, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if(rank==0){
        printf("Process %d has total sum: %d\n", rank, globalSum);
    }

    MPI_Finalize();
    return 0;
}
