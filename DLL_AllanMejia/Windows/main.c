#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	int num1,num2;
	printf("Ingrese primer numero: ");
	scanf("%d",&num1);
	printf("Ingrese segundo numero: ");
	scanf("%d",&num2);
	printf("%d\n",add(num1,num2));
	system("pause");
	return 1;
}