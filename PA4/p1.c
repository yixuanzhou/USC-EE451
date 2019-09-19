#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

main(int argc, char *argv[])
{
    int size, rank;
    int new_size, new_rank;
    int number;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    printf("COMM_WORLD: From process %d out of %d\n", rank, size);

    if(rank != 0){
			MPI_Recv(&number, 1, MPI_INT, rank-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      number += 1;
      printf("Process %d received Msg = %d\n", rank, number);
    } else {
      number = 451;
      printf("Process %d initialized Msg = %d\n", rank, number);
    }

    MPI_Send(&number, 1, MPI_INT, (rank+1)%size, 0, MPI_COMM_WORLD);

    if (rank == 0) {
      MPI_Recv(&number, 1, MPI_INT, size-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      printf("Process %d Received Msg = %d. Done!\n", rank, number);
    }

    MPI_Finalize();
}
