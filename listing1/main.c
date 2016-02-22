
#include "common.h"
#include <stdio.h>

int main(int argc, char** argv)
{
	void* p1 = malloc(10);
	void* p2 = malloc(10);
	void* p3 = malloc(10);
	printf("Allocated memory: %llu\n", (unsigned long long)get_allocated_memory());
	free(p2);
	free(p3);
	printf("Allocated memory: %llu\n", (unsigned long long)get_allocated_memory());
	return 0;
}
