#include "pthread.h"
#include <stdio.h>
#include <stdlib.h>

int fib; 
static void* fibonacci_thread( void* arg );

int main()
{
	int number;
	printf("Ingresar numero\n");
     	scanf("%d", &number);
	printf("Fibonacci de %d es %d\n", number, (int)fibonacci_thread((void*)number));
}

static void* fibonacci_thread( void* arg ) {
int n = (int)arg, fib;

pthread_t thread1, thread2;

void* value;

if(n <= 1)
	return (void*)1;

pthread_create(&thread1, NULL, fibonacci_thread, (void*)(n-1));
pthread_create( &thread2, NULL, fibonacci_thread, (void*)(n-2));
pthread_join(thread1, &value);
fib = (int)value;
pthread_join(thread2, &value);
fib += (int)value;
return (void*)fib;
}


