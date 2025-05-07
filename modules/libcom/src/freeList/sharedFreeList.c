#include "cantProceed.h"
#include "freeList.h"
#include "epicsMutex.h"
#include "sharedFreeList.h"

typedef struct sharedHeader {
    long int ref;
    epicsMutexId mutex;
    void *mem;
}sharedHeader;

typedef struct {
    void (*dtor)(void *);
    void *pvt;
}sharedFreeListPvt;

LIBCOM_API void epicsStdCall sharedFreeListInitPvt(void **ppvt, void (*dtor)(void *), int size, int malloc) {
    sharedFreeListPvt *pvt = callocMustSucceed(1, sizeof(sharedFreeListPvt), "Failed to allocate shared free list");
    if (pvt == NULL){
        return;
    }
    freeListInitPvt(&pvt->pvt, size+sizeof(sharedHeader), malloc);
    if (pvt->pvt == NULL){
        return;
    }
    pvt->dtor=dtor;
    *ppvt=pvt;
}

LIBCOM_API void * epicsStdCall sharedFreeListCalloc(void *pvt) {
    sharedFreeListPvt *sharedPvt = (sharedFreeListPvt *)pvt;
    sharedHeader *pfl = (sharedHeader *)freeListCalloc(sharedPvt->pvt);
    pfl->ref=1;
    pfl->mutex = epicsMutexCreate();
    return &pfl->mem;
}

LIBCOM_API void * epicsStdCall sharedFreeListMalloc(void *pvt) {
    sharedFreeListPvt *sharedPvt = (sharedFreeListPvt *)pvt;
    sharedHeader *pfl = (sharedHeader *)freeListMalloc(sharedPvt->pvt);
    pfl->ref=1;
    pfl->mutex = epicsMutexCreate();
    return &pfl->mem;
}

LIBCOM_API enum sharedReleaseStatus epicsStdCall sharedFreeListRelease(void *pvt, void *pmem) {
    sharedFreeListPvt *sharedPvt = (sharedFreeListPvt *)pvt;
    sharedHeader *pfl = pmem-(sizeof(sharedHeader)-sizeof(void *));
    epicsMutexLock(pfl->mutex);
    pfl->ref--;
    if (pfl->ref <= 0) {
        epicsMutexUnlock(pfl->mutex);
        sharedPvt->dtor(pmem);
        freeListFree(sharedPvt->pvt, pfl);
        return SHR_MEM_RELEASED;
    }
    epicsMutexUnlock(pfl->mutex);
    return SHR_MEM_NOT_RELEASED;
}

LIBCOM_API void epicsStdCall sharedFreeListAcquire(void *pmem) {
    sharedHeader *pfl = pmem-(sizeof(sharedHeader)-sizeof(void *));
    epicsMutexLock(pfl->mutex);
    pfl->ref++;
    epicsMutexUnlock(pfl->mutex);
}

LIBCOM_API void epicsStdCall sharedFreeListCleanup(void *pvt) {
    sharedFreeListPvt *sharedPvt = (sharedFreeListPvt *)pvt;
    freeListCleanup(sharedPvt->pvt);
    free(sharedPvt);
}

LIBCOM_API size_t epicsStdCall sharedFreeListItemsAvail(void *pvt){
    sharedFreeListPvt *sharedPvt = (sharedFreeListPvt *)pvt;
    return freeListItemsAvail(sharedPvt->pvt);
}
