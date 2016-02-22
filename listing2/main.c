
#include "common.h"
#include <stdio.h>

typedef struct {
	int nRefCount;
	void* pResource;
} sCacheResource;

sCacheResource** g_resourceCache = NULL;
int g_nResourceCacheCurrentSize = 0;
int g_nNextFreeSlot = 0;

//Oops - free slots aren't reused meaning the g_resourceCache array grows unnecessarily
int allocateNewResource(size_t nResSize)
{
	int nRet = g_nNextFreeSlot;
	if (g_nResourceCacheCurrentSize < (g_nNextFreeSlot + 1)) {
		int nNewSize = (g_nResourceCacheCurrentSize == 0) ? 2 : (g_nResourceCacheCurrentSize*2);
		g_resourceCache = realloc(g_resourceCache, nNewSize*sizeof(sCacheResource*));
		g_nResourceCacheCurrentSize = nNewSize;
	}
	g_resourceCache[g_nNextFreeSlot] = (sCacheResource*)malloc(sizeof(sCacheResource));
	g_resourceCache[g_nNextFreeSlot]->pResource = malloc(nResSize);
	g_resourceCache[g_nNextFreeSlot]->nRefCount = 1;
	g_nNextFreeSlot++;
	return nRet;
}

void releaseCacheReference(int nSlot) {
	g_resourceCache[nSlot]->nRefCount--;
	if (g_resourceCache[nSlot]->nRefCount==0) {
		free(g_resourceCache[nSlot]->pResource);
		free(g_resourceCache[nSlot]);
		g_resourceCache[nSlot] = NULL;
	}
}

void cleanupOnShutdown()
{
	for (int i = 0; i < g_nNextFreeSlot; i++)
	{
		if (g_resourceCache[i] != NULL) {
			if (g_resourceCache[i]->pResource != NULL) {
				free(g_resourceCache[i]->pResource);
			}
			free(g_resourceCache[i]);
		}
	}
}

int g_ShowSlots[4];

void Show()
{
	g_ShowSlots[0] = allocateNewResource(10);
	g_ShowSlots[1] = allocateNewResource(10);
	g_ShowSlots[2] = allocateNewResource(10);
	g_ShowSlots[3] = allocateNewResource(10);
	// Oops - reference counting error means this resource won't get freed until shutdown
	g_resourceCache[g_ShowSlots[2]]->nRefCount++;
}

void Hide()
{
	releaseCacheReference(g_ShowSlots[0]);
	releaseCacheReference(g_ShowSlots[1]);
	releaseCacheReference(g_ShowSlots[2]);
	releaseCacheReference(g_ShowSlots[3]);
}

int main(int argc, char** argv)
{
	initDump();
	Show();
	Hide();
	endDump();
	cleanupOnShutdown();
	return 0;
}
