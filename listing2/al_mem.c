
#include "common.h"
#include <stdio.h>

#undef malloc
#undef calloc
#undef realloc
#undef free
#undef strdup


static size_t g_nAllocatedMemory = 0;
static FILE* g_hDumpFile = NULL;

size_t get_allocated_memory()
{
	return g_nAllocatedMemory;
}

void initDump()
{
	g_hDumpFile = fopen("memdump.txt", "w");
}

void endDump()
{
	fclose(g_hDumpFile);
	g_hDumpFile = NULL;
}

void *al_malloc(size_t nSize)
{
	//Avoid weird things happening when nSize + sizeof(size_t) overflows
	if (nSize >= (size_t)(0 - sizeof(size_t))) {
		return NULL;
	}
	void* ptr = malloc(nSize + sizeof(size_t));
	if (ptr == NULL) {
		return NULL;
	}
	*(size_t*)ptr = nSize;
	g_nAllocatedMemory += nSize;
	return (void*)((char*)ptr + sizeof(size_t));
}

void *al_calloc(size_t nElemCount, size_t nElemSize)
{
	size_t nSize = nElemCount*nElemSize;
	if (nElemSize != 0 && nSize / nElemSize != nElemCount) {
		// overflow handling
		return NULL;
	}
	void* ptr = al_malloc(nSize);
	if (ptr != NULL) {
		memset(ptr, 0, nSize);
	}
	return ptr;
}

void *al_realloc(void *ptr, size_t nSize)
{
	if (ptr == NULL) {
		return al_malloc(nSize);
	}
	//Avoid weird things happening when nSize + sizeof(size_t) overflows
	if (nSize >= (size_t)(0 - sizeof(size_t))) {
		return NULL;
	}
	size_t nOldSize = *(size_t*)((char*)ptr - sizeof(size_t));
	g_nAllocatedMemory = g_nAllocatedMemory - nOldSize + nSize;
	void* ptrNew = realloc((void*)((char*)ptr - sizeof(size_t)), nSize + sizeof(size_t));
	if (ptrNew == NULL) {
		return NULL;
	}
	*(size_t*)ptrNew = nSize;
	return (void*)((char*)ptrNew + sizeof(size_t));
}

void al_free(void *ptr)
{
	size_t nSize = *(size_t*)((char*)ptr - sizeof(size_t));
	g_nAllocatedMemory = g_nAllocatedMemory - nSize;
	free((void*)((char*)ptr - sizeof(size_t)));
}

char *al_strdup(const char *s)
{
	size_t nSize = strlen(s) + 1;
	void* ptr = al_malloc(nSize);
	if (ptr) {
		strcpy_s(ptr, nSize, s);
	}
	return (char*)ptr;
}

#ifdef MEMDUMP

void *al_malloc_memdump(size_t nSize, char* strFileName, int nLineNumber)
{
	void *ptr = al_malloc(nSize);
	if (g_hDumpFile) {
		fprintf(g_hDumpFile, "M,%p,%lu,%s:%d\n", ptr, (unsigned long)nSize, strFileName, nLineNumber);
	}
	return ptr;
}

void *al_calloc_memdump(size_t nElemCount, size_t nElemSize, char* strFileName, int nLineNumber)
{
	void *ptr = al_calloc(nElemCount, nElemSize);
	if (g_hDumpFile) {
		fprintf(g_hDumpFile, "C,%p,%lu,%s:%d\n", ptr, (unsigned long)(nElemCount*nElemSize), strFileName, nLineNumber);
	}
	return ptr;
}

void *al_realloc_memdump(void *ptr, size_t nSize, char* strFileName, int nLineNumber)
{
	void *ptrNew = al_realloc(ptr, nSize);
	if (g_hDumpFile) {
		fprintf(g_hDumpFile, "R,%p,%p,%lu,%s:%d\n", ptr, ptrNew, (unsigned long)nSize, strFileName, nLineNumber);
	}
	return ptrNew;
}

void al_free_memdump(void *ptr, char* strFileName, int nLineNumber)
{
	al_free(ptr);
	if (g_hDumpFile) {
		fprintf(g_hDumpFile, "F,%p,%s:%d\n", ptr, strFileName, nLineNumber);
	}
}

char *al_strdup_memdump(const char *s, char* strFileName, int nLineNumber)
{
	char * sNew = al_strdup(s);
	if (g_hDumpFile) {
		fprintf(g_hDumpFile, "S,%p,%s:%d\n", sNew, strFileName, nLineNumber);
	}
	return sNew;
}

#endif
