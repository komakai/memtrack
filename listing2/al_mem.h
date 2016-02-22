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

#ifdef MEMDUMP

void initDump();
void endDump();

void *al_malloc_memdump(size_t nSize, char* strFileName, int nLineNumber);
void *al_calloc_memdump(size_t nElemCount, size_t nElemSize, char* strFileName, int nLineNumber);
void *al_realloc_memdump(void *ptr, size_t nSize, char* strFileName, int nLineNumber);
void al_free_memdump(void *ptr, char* strFileName, int nLineNumber);
char *al_strdup_memdump(const char *s, char* strFileName, int nLineNumber);

#define malloc(x)      al_malloc_memdump(x,__FILE__,__LINE__)
#define calloc(x,y)    al_calloc_memdump(x,y,__FILE__,__LINE__)
#define realloc(x,y)   al_realloc_memdump(x,y,__FILE__,__LINE__)
#define free(x)        al_free_memdump(x,__FILE__,__LINE__)
#define strdup(x)      al_strdup_memdump(x,__FILE__,__LINE__)

#else

#define malloc(x)      al_malloc(x)
#define calloc(x,y)    al_calloc(x,y)
#define realloc(x,y)   al_realloc(x,y)
#define free(x)        al_free(x)
#define strdup(x)      al_strdup(x)

#endif

#endif //AL_MEM_H
