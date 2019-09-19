#include <stdlib.h>
#include <stdio.h>
#include <cublas.h>
#include <time.h>

#define HEIGHT 1024
#define WIDTH 1024
#define BLOCK_SIZE 32


__global__ void matrix_mult(int *a, int *b, int *c){
	int threadRow = threadIdx.y;
	int threadCol = threadIdx.x;

	int row = blockIdx.y * blockDim.y + threadIdx.y;
	int col = blockIdx.x * blockDim.x + threadIdx.x;

	int c_val = 0;
	for (int i = 0; i<(WIDTH/BLOCK_SIZE); i++) {
		__shared__ int a_share[BLOCK_SIZE][BLOCK_SIZE];
		__shared__ int b_share[BLOCK_SIZE][BLOCK_SIZE];

		// each thread reads one element from both A and B matrices into the shared sub-matrices
		a_share[threadRow][threadCol] = a[row * WIDTH + col];
		b_share[threadRow][threadCol] = b[row * WIDTH + col];

		// make sure the sub-matrices are loaded before starting the computation
		__syncthreads();

		for (int i=0; i<BLOCK_SIZE; i++) {
			c_val += a_share[threadRow][i] * b_share[i][threadCol];
		}

		// make sure every thread is done computing before loading new sub-matrices
		__syncthreads();

	}

	c[row * WIDTH + col] = c_val;
}

int main(){
    int i;
    int *a = (int*)malloc(sizeof(int) * HEIGHT * WIDTH);
	int *b = (int*)malloc(sizeof(int) * HEIGHT * WIDTH);
    int *c = (int*)malloc(sizeof(int) * HEIGHT * WIDTH);
	for(i=0; i<WIDTH * HEIGHT; i++){
		a[i]=1;
		b[i]=2;
  	}

	int *gpu_a, *gpu_b, *gpu_c;
	cudaMalloc((void**)&gpu_a, sizeof(int) * HEIGHT * WIDTH);
	cudaMalloc((void**)&gpu_b, sizeof(int) * HEIGHT * WIDTH);
	cudaMalloc((void**)&gpu_c, sizeof(int) * HEIGHT * WIDTH);

	struct timespec start, stop;
	double time;

	cudaMemcpy(gpu_a, a, sizeof(int) * HEIGHT * WIDTH, cudaMemcpyHostToDevice);
	cudaMemcpy(gpu_b, b, sizeof(int) * HEIGHT * WIDTH, cudaMemcpyHostToDevice);

	dim3 dimGrid(32, 32);
	dim3 dimBlock(32, 32);

	if( clock_gettime( CLOCK_REALTIME, &start) == -1 ) { perror( "clock gettime" );}

	matrix_mult<<<dimGrid, dimBlock>>>(gpu_a, gpu_b, gpu_c);
	cudaMemcpy(c, gpu_c, sizeof(int) * HEIGHT * WIDTH, cudaMemcpyDeviceToHost);

	if( clock_gettime( CLOCK_REALTIME, &stop) == -1 ) { perror( "clock gettime" );}
	time = (stop.tv_sec - start.tv_sec)+ (double)(stop.tv_nsec - start.tv_nsec)/1e9;
	printf("time is %f ns\n", time*1e9);

	printf("c[451][451]=%d\n", c[451*1024+451]);

	free(a);
	free(b);
	free(c);
	cudaFree(gpu_a);
	cudaFree(gpu_b);
	cudaFree(gpu_c);
	return 0;
}
