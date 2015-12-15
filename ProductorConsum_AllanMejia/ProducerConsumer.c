#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include "Semaforo.h"

typedef int buffer_item;
#define BUFFER_SIZE 5

buffer_item buffer[BUFFER_SIZE];

int insert_item(buffer_item item) {
	
	for (int i = 0; i < BUFFER_SIZE; i++)
	{
		if(buffer[i] == -1){
			buffer[i] = item;
			printf ("producer produced %d\n", item);
			return 0;
		}
	}
	return -1;
}

int remove_item(buffer_item *item) {
	for (int i = 0; i < BUFFER_SIZE; i++)
	{
		if(buffer[i] != -1){
			*item = buffer[i];
			buffer[i] = -1;
			printf("consumer consumed %d\n", *item);
			return 0;
		}
	}
	return -1;
}

void *producer(void *param);
void *consumer(void *param);
void create_producers();
void create_consumers();
pthread_mutex_t mutex;
struct Semaforo sem_a;
struct Semaforo sem_p;
sem_t sem_available;
sem_t sem_produced;

int main(int argc, char*argv[]) {
	/* 1. Get command line arguments argv[1], argv[2], argv[3] */
	//1. How long to sleep before terminating.	
	//2. The number of producer threads
	//3. The number of consumer threads
	int seconds , nproducerthreads, nconsumerthreads;
	printf("Ingrese numero de segundos a dormir: ");	
	scanf("%d",&seconds);
	printf("Ingrese numero de hilos de productor: ");
	scanf("%d",&nproducerthreads);
	printf("Ingrese numero de hilos de consumidor: ");
	scanf("%d",&nconsumerthreads);
	/*seconds = argv[1] - '0';
	nproducerthreads = argv[2] - '0';
	nconsumerthreads = argv[3] - '0';
	printf("%d %d %d", seconds , nproducerthreads,nconsumerthreads);
	/* 2. Initialize buffer, mutex, semaphores, and other global vars */
	for (int i = 0; i < BUFFER_SIZE; i++)
	{
		buffer[i] = -1;
	}
	set(BUFFER_SIZE,&sem_a);
	set(0,&sem_p);
	sem_init(&sem_available, 0, BUFFER_SIZE);
	sem_init(&sem_produced, 0, 0);
	pthread_mutex_init(&mutex, NULL);
	/* 3. Create producer thread(s) */
	for(int i = 0; i < nproducerthreads;i++)
		create_producers();
	/* 4. Create consumer thread(s) */
	for(int i = 0; i < nconsumerthreads;i++)
		create_consumers();
	/* 5. Sleep */
	sleep (seconds);
	/* 6. Release resources, e.g. destroy mutex and semaphores */
	pthread_mutex_destroy(&mutex);
	sem_destroy(&sem_available);
	sem_destroy(&sem_produced);
	/* 7. Exit */
	exit(0);
	
}

void create_producers(){
	pthread_t tid;
	pthread_attr_t attr;
	
	pthread_attr_init(&attr);
	
	pthread_create(&tid, &attr, producer, NULL);
}

void create_consumers(){
	pthread_t tid;
	pthread_attr_t attr;

	pthread_attr_init(&attr);

	pthread_create(&tid, &attr, consumer, NULL);
}

void *producer(void *param) {
	unsigned int seed = time(NULL);
	buffer_item rand;
	int seconds;
	while (1) {
		seconds = rand_r(&seed) % 10;
		sleep(seconds);

		//sem_wait(&sem_available);
		wait(&sem_a);
		pthread_mutex_lock(&mutex);
		
		rand = rand_r(&seed) % 100;
		if (insert_item(rand) < 0)
			printf("Error producing item\n");

		pthread_mutex_unlock(&mutex);
		//sem_post(&sem_produced);
		signal(&sem_p);
	}
}

void *consumer(void *param) {
	unsigned int seed = time(NULL);
	buffer_item rand;
	int seconds;
	while (1) {
		seconds = rand_r(&seed) % 15;
		sleep (seconds);

		//sem_wait(&sem_produced);
		wait(&sem_p);
		pthread_mutex_lock(&mutex);

		rand = rand_r(&seed) % 100;
		if (remove_item(&rand) < 0)
			printf("Error consuming item\n");

		pthread_mutex_unlock(&mutex);
		//sem_post(&sem_available);
		signal(&sem_a);
	}
}
