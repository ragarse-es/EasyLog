/**
 * @file
 * @brief RTK shared memory functions
 * @ingroup rtk
 */

#ifndef _RTK_SHARED_MEMORY_H_
#define _RTK_SHARED_MEMORY_H_

#include <rtkBase.h>

#ifdef __cplusplus
extern "C" {
#endif

/** @brief RTK shared memory handle */
typedef RTK_H RTK_SHARED_MEM_H;

/**
 * @brief Creates a shared memory object with the provided name ('lpszSharedMemoryName' must be unique or NULL).
 * 
 * Allocates memory of size 'ulByteLength'.
 * A handle to the shared memory is returned as output-parameter ('hSharedMemory' - must not be NULL) */
EXIMPORT RTK_ERROR
RtkCreateSharedMemory(const char* lpszSharedMemoryName, ULONG ulByteLength, RTK_SHARED_MEM_H* hSharedMemory);

/** @brief Deletes a shared memory object */
EXIMPORT RTK_ERROR RtkDeleteSharedMemory(RTK_SHARED_MEM_H hSharedMemory);

/**
 * @brief Registers for usage of the shared memory.
 * 
 * Obtains the memory address ('lpMemoryAddress' - must not be NULL) of the shared memory. Data can then be read/written
 * to this address. */
EXIMPORT RTK_ERROR RtkMapSharedMemory(RTK_SHARED_MEM_H hSharedMemory, void** lpMemoryAddress);

/**
 * @brief Unregisters usage of this shared memory. */
EXIMPORT RTK_ERROR RtkUnmapSharedMemory(RTK_SHARED_MEM_H hSharedMemory);

#ifdef __cplusplus
}
#endif

#endif
