/**
 * @file
 * @brief RTK memory functions
 * @ingroup rtk
 */

#ifndef _RTK_MEMORY_H_
#define _RTK_MEMORY_H_

#include <rtkBase.h>

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Type of RTK memory access flags */
typedef unsigned long RTK_MEMORY_ACCESS;

/** @name Memory access flags */ 
/*@{*/
#define RTK_MEM_NO_READ 0x00000000       /**< @brief ignored */
#define RTK_MEM_NO_WRITE 0x00000000      /**< @brief ignored */
#define RTK_MEM_NOT_EXECUTE 0x00000000   /**< @brief ignored */
#define RTK_MEM_READ_ALL 0x00000001      /**< @brief ignored */
#define RTK_MEM_WRITE_ALL 0x00000002     /**< @brief ignored */
#define RTK_MEM_EXECUTE_ALL 0x00000004   /**< @brief ignored */
#define RTK_MEM_READ_LOCAL 0x00000008    /**< @brief ignored */
#define RTK_MEM_WRITE_LOCAL 0x00000010   /**< @brief ignored */
#define RTK_MEM_EXECUTE_LOCAL 0x00000020 /**< @brief ignored */
#define RTK_MEM_NO_INIT 0x00000000       /**< @brief no memory initialization */
#define RTK_MEM_INITZERO 0x00008000      /**< @brief memory initialization with 0 */
/*@}*/

/**
 * @brief Allocates heap memory.
 * 
 * Depending on the 'AccessFlags', the memory will be initialized with 0 (#RTK_MEM_INITZERO) or it will be not initialized
 * (#RTK_MEM_NO_INIT). After the function call 'lpMemoryAddress' (which must not be NULL) contains the pointer to the
 * allocated memory */
EXIMPORT RTK_ERROR RtkAllocateMemory(ULONG ulByteLength, RTK_MEMORY_ACCESS AccessFlags, void** lpMemoryAddress);

/**
 * @brief Frees memory allocated with RtkAllocateMemory().
 * 
 * 'lpMemoryaddress' is the pointer to the memory to be freed */
EXIMPORT RTK_ERROR RtkFreeMemory(void* lpMemoryAddress, ULONG ulUnused);

/**
 * @brief Retrieves information about the DRAM
 * @param lpulMemSize              Output-parameter - can be NULL. Returns the overall RAM size in byte.
 * @param lpulFreeMemSize          Output-parameter - can be NULL. Returns the available/free RAM size in byte.
 * @param lpulBiggestFreeBlockSize Output-parameter - can be NULL. Returns the  biggest available/free consecutive
 *                                                              RAM block in byte. */
EXIMPORT RTK_ERROR RtkGetMemoryInfo(ULONG* lpulMemSize, ULONG* lpulFreeMemSize, ULONG* lpulBiggestFreeBlockSize);

#ifdef __cplusplus
}
#endif

#endif
