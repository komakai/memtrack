#ifndef AL_MEM_H
#define AL_MEM_H

#include <stdlib.h>
#include <string.h>

void *al_malloc(size_t size);
void *al_calloc(size_t nelem, size_t elsize);
void *al_realloc(void *ptr, size_t size);
void al_free(void *ptr);
char *al_strdup(const char *s);

size_t get_allocated_memory();

#define malloc(x)      al_malloc(x)
#define calloc(x,y)    al_calloc(x,y)
#define realloc(x,y)   al_realloc(x,y)
#define free(x)        al_free(x)
#define strdup(x)      al_strdup(x)

#endif //AL_MEM_H
