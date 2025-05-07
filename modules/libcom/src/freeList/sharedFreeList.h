/*************************************************************************\
* Copyright (c) 2002 The University of Chicago, as Operator of Argonne
*     National Laboratory.
* Copyright (c) 2002 The Regents of the University of California, as
*     Operator of Los Alamos National Laboratory.
* SPDX-License-Identifier: EPICS
* EPICS Base is distributed subject to a Software License Agreement found
* in file LICENSE that is included with this distribution.
\*************************************************************************/
/**
 * \file sharedFreeList.h
 * \author Lucas A. M. Magalhães
 *
 * \brief Allocate and free fixed size memories with reference counting
 *
 * \details
 * Describes routines to allocate and free fixed size memory elements.
 * Free elements are maintained on a free list rather than being returned to the heap via calls to free.
 * When it is necessary to call malloc(), memory is allocated in multiples of the element size.
 */

#ifndef INCsharedFreeListh
#define INCsharedFreeListh

#include <stddef.h>
#include "libComAPI.h"

#ifdef __cplusplus
extern "C" {
#endif

enum sharedReleaseStatus{
    SHR_MEM_RELEASED,
    SHR_MEM_NOT_RELEASED
};

LIBCOM_API void epicsStdCall sharedFreeListInitPvt(void **ppvt, void (*dtor)(void *), int size, int malloc);
LIBCOM_API void * epicsStdCall sharedFreeListCalloc(void *pvt);
LIBCOM_API void * epicsStdCall sharedFreeListMalloc(void *pvt);
LIBCOM_API enum sharedReleaseStatus epicsStdCall sharedFreeListRelease(void *pvt, void *pmem);
LIBCOM_API void epicsStdCall sharedFreeListAcquire(void *pmem);
LIBCOM_API void epicsStdCall sharedFreeListCleanup(void *pvt);
LIBCOM_API size_t epicsStdCall sharedFreeListItemsAvail(void *pvt);

#ifdef __cplusplus
}
#endif

#endif /*INCsharedFreeListh*/
