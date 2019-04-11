#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define		num_of_elements	   16*1024*1024

void quickSort(int *array, int start, int end){
	int i=start, j=end;
	int tmp;
	int pivot =array[(start+end)/2];
	while(i<=j){
		while(array[i]<pivot)  i++;
		while(array[j]>pivot)  j--;
		if(i<=j){
			tmp = array[i];
			array[i] = array[j];
			array[j] = tmp;
			i++; j--;
		}
	}
	if(start<j)
		quickSort(array,start,j);
	if(i<end)
		quickSort(array,i,end);
}

int power(int k){
	int i, result = 1;
	for(i=0;i<k;i++){
		result *=2;	
	}		
	return result;
}
int main(void){
	int i, j, tmp;
	struct timespec start, stop; 
	double exe_time;
	srand(time(NULL)); 
	int * m = (int *) malloc (sizeof(int)*num_of_elements);
	for(i=0; i<num_of_elements; i++){
		m[i]=num_of_elements-i;
	}
	if( clock_gettime(CLOCK_REALTIME, &start) == -1) { perror("clock gettime");}


	////////**********Your code goes here***************//
/*
	i=0;
	j=num_of_elements-1;
	int pivot =m[rand()%num_of_elements];
	printf("pivot %d\n", pivot);
	while(i<=j){
		while(m[i]<pivot)  i++;
		while(m[j]>pivot)  j--;
		if(i<=j){
			tmp = m[i];
			m[i] = m[j];
			m[j] = tmp;
			i++; 
			j--;
		}
	}
*/   	
	//rintf("i=%d, j=%d\n", i,j);
	quickSort(m, 0, num_of_elements -1);	
	//quickSort(m, i, num_of_elements-1);	
	//quickSort(m, 0, num_of_elements-1);	

	///////******************************////

	if( clock_gettime( CLOCK_REALTIME, &stop) == -1 ) { perror("clock gettime");}		
	exe_time = (stop.tv_sec - start.tv_sec)+ (double)(stop.tv_nsec - start.tv_nsec)/1e9;

	for(i=0;i<100;i++) printf("%d ", m[i]);		
	printf("\nExecution time = %f sec\n",  exe_time);		
}	
