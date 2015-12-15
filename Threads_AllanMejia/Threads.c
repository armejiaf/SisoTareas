#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
     
struct SortParameters{
    int *arr;
    int begin;
    int end;
};
          
void buildRandomArray(int *arr,int size);
void * sortArrayBySegment(void * params);
void printArray(int *arr,int size);
void merge_sort (int *a, int size); 
void merge (int *a, int size, int m);
    
int main(){

     	int size=0;
    	int threads=0;
	printf("Ingresar tamaño del arreglo: ");
	scanf("%d", &size);
	printf("Ingresar cantidad de hilos a utilizar: ");
	scanf("%d", &threads);
	
 	if(size % threads != 0){
          printf("El tamaño del arreglo y la cantidad de hilos tienen que ser divisible entre si.\n");
          return -1;
        }
	if(size < threads){
          printf("La cantidad de hilos no puede ser mayor a el tamaño del arreglo.\n");
          return -1;
        }
   	
	int *arr=(int *)malloc(sizeof(int)*size);
	buildRandomArray(arr,size);
	
	int topSize = size / threads;
	
	struct SortParameters params;
	pthread_t tid[threads];
	pthread_attr_t attr;
	pthread_attr_init(&attr);
       
	params.begin = 0;
	params.end = topSize;                
	for (int i = 0; i < threads; i++)
	{      
	    params.arr = arr;
	    pthread_create(&tid[i],&attr,sortArrayBySegment,&params);
	    params.begin+= topSize;
	    params.end+= topSize;
	    pthread_join(tid[i],0);
	}
    	printArray(arr,size);
   	merge_sort(arr,size);
	printf("\n");   	
	printArray(arr,size);
    	printf("\n");
    	return 0;
}
     
void buildRandomArray(int *arr,int size){
      for(int i =0;i<size;i++){
	            arr[i]= rand()%1000+1;
      }
}
     
void * sortArrayBySegment(void * params){
    	printf("%s\n","Corriendo hilo...." );
    	struct SortParameters *parameters =(struct SortParameters*)params;
    	for(int i=parameters->begin;i<parameters->end;i++){
            	for(int j=parameters->begin;j<parameters->end;j++){
                    	if(parameters->arr[i]<parameters->arr[j]){
                            	int tmp=parameters->arr[i];
                            	parameters->arr[i]=parameters->arr[j];
                            	parameters->arr[j]=tmp;
                    	}
            	}
    	}
    	pthread_exit(0);
}
     
void printArray(int *arr,int size){
	for(int i =0;i<size;i++){
		printf("%d ",arr[i]);
	}
}

void merge_sort (int *a, int size) {
    	if (size < 2)
        	return;
    	int m = size / 2;
    	merge_sort(a, m);
    	merge_sort(a + m, size - m);
    	merge(a, size, m);
}
 
void merge (int *a, int size, int m) {
    	int i, j, k;
    	int *x = malloc(size * sizeof (int));
    	for (i = 0, j = m, k = 0; k < size; k++) {
           	x[k] = j == size      ? a[i++]
             		: i == m      ? a[j++]
             		: a[j] < a[i] ? a[j++]
             		:               a[i++];
    	}
    	for (i = 0; i < size; i++) {
        	a[i] = x[i];
    	}
    	free(x);
}
