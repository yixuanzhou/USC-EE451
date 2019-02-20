#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int min(a, b) {
      return a < b ? a : b;
}

int main(void){
		int i, j, k;
		struct timespec start, stop;
		double time;
		int n = 4096; // matrix size is n*n

		double **A = (double**) malloc (sizeof(double*)*n);
		double **B = (double**) malloc (sizeof(double*)*n);
		double **C = (double**) malloc (sizeof(double*)*n);
		for (i=0; i<n; i++) {
			A[i] = (double*) malloc(sizeof(double)*n);
			B[i] = (double*) malloc(sizeof(double)*n);
			C[i] = (double*) malloc(sizeof(double)*n);
		}

		for (i=0; i<n; i++){
			for(j=0; j< n; j++){
				A[i][j]=i;
				B[i][j]=i+j;
				C[i][j]=0;
			}
		}

    int b = 4; // block size
		int bk, bj;

		if( clock_gettime(CLOCK_REALTIME, &start) == -1) { perror("clock gettime");}

		// Your code goes here //
		// Matrix C = Matrix A * Matrix B //
		//*******************************//
		for (bk = 0; bk < n; bk += b) {
			for (bj = 0; bj < n; bj += b) {
				for (i = 0; i < n; i++) {
					for (j = bj; j < min(bj+b, n); j++) {
						for (k = bk; k < min(bk+b, n); k++) {
							C[i][j] = C[i][j] + A[i][k] * B[k][j];
            }
          }
				}
			}
		}
		//*******************************//

		if( clock_gettime( CLOCK_REALTIME, &stop) == -1 ) { perror("clock gettime");}
		time = (stop.tv_sec - start.tv_sec)+ (double)(stop.tv_nsec - start.tv_nsec)/1e9;

    printf("Block size is %d\n", b);
		printf("Number of FLOPs = %lu, Execution time = %f sec,\n%lf MFLOPs per sec\n", 2*n*n*n, time, 1/time/1e6*2*n*n*n);
		printf("C[100][100]=%f\n", C[100][100]);

		// release memory
		for (i=0; i<n; i++) {
			free(A[i]);
			free(B[i]);
			free(C[i]);

		}
		free(A);
		free(B);
		free(C);
		return 0;
}
