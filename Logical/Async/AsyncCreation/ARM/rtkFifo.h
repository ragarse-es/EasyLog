/**
 * @file
 * @brief RTK FIFO functions
 * @ingroup rtk
 */

#ifndef _RTK_FIFO_H_
#define _RTK_FIFO_H_

#include <rtkBase.h>

#ifdef __cplusplus
extern "C" {
#endif

/** @brief RTK FIFO handle */
typedef RTK_H RTK_FIFO_H;

/** @brief Type of FIFO flags */
typedef unsigned long RTK_FIFOFLAG;

/** @name Fifo flags (create) */ 
/*@{*/
#define RTK_FIFO_WRITE_UNTIL_FULL 0x00000000 /**< @brief no overwrite */
#define RTK_FIFO_WRITE_ALWAYS 0x00000001     /**< @brief overwrite */
#define RTK_FIFO_ATTACH 0x00000002           /**< @brief FIFO access only attach/release */
/*@}*/

/**
 * @brief Creates a FIFO with the provided name ('lpszFifoName' must be unique or NULL) and the provided size ('ulFifoByteSize').
 * 
 * The 'FifoFlags' determine whether RtkWriteFifo calls will overwrite data in the FIFO (#RTK_FIFO_WRITE_ALWAYS) or whether
 * RtkWriteFifo calls only succeed if there is enough free space available in the FIFO (#RTK_FIFO_WRITE_UNTIL_FULL).
 * A handle to the FIFO is returned as output-parameter ('hFifo' - must not be NULL) */
EXIMPORT RTK_ERROR
RtkCreateFifo(const char* lpszFifoName, ULONG ulFifoByteSize, RTK_FIFOFLAG FifoFlags, RTK_FIFO_H* hFifo);

/** @brief Deletes a FIFO. */
EXIMPORT RTK_ERROR RtkDeleteFifo(RTK_FIFO_H hFifo);

/**
 * @brief Writes data ('lpDataSource') with size 'ulDataByteSize' to the FIFO.
 * 
 * If the FIFO was created with the flag #RTK_FIFO_WRITE_ALWAYS, then unread data will be overwritten.
 * If the FIFO was created with the flag #RTK_FIFO_WRITE_UNTIL_FULL, then unread data will not be overwritten and this
 * function waits for 'ulTimeoutUsec' (RTK_TIMEOUT_WAITFOREVER to wait forever) to put the data into the FIFO and
 * returns #ERR_RTK_TIMEOUT if the timeout expires. */
EXIMPORT RTK_ERROR RtkWriteFifo(RTK_FIFO_H hFifo, const void* lpDataSource, ULONG ulDataByteSize, ULONG ulTimeoutUsec);

/**
 * @brief Copies data of size 'lpDataByteSize' from the FIFO to 'lpDataDest' (must not be NULL).
 * 
 * Waits for 'ulTimeoutUsec' (RTK_TIMEOUT_WAITFOREVER to wait forever) if not enough data is available in the FIFO. */
EXIMPORT RTK_ERROR RtkReadFifo(RTK_FIFO_H hFifo, void* lpDataDest, ULONG lpDataByteSize, ULONG ulTimeoutUsec);

/**
 * @brief Deletes all data in the FIFO. */
EXIMPORT RTK_ERROR RtkClearFifo(RTK_FIFO_H hFifo);

/**
 * @brief Locks access (read/write) to the FIFO for other tasks.
 * 
 * It is possible to have one task reading and one task writing to the FIFO without requiring locking with this
 * function. However, if multiple tasks read or if multiple tasks write to the FIFO, then locking with this function is
 * required. */
EXIMPORT RTK_ERROR RtkAttachFifo(RTK_FIFO_H hFifo);

/** @brief Unlocks access (read/write) to the FIFO for other tasks. */
EXIMPORT RTK_ERROR RtkReleaseFifo(RTK_FIFO_H hFifo);

/**
 * @brief Obtains information about the FIFO.
 * @param hFifo        Handle to the FIFO for which the information shall be retrieved.
 * @param lpulFifoFree Output-parameter - can be NULL. Returns the number of free bytes.
 * @param lpulFifoUsed Output-parameter - can be NULL. Returns the number of occupied bytes. */
EXIMPORT RTK_ERROR RtkGetFifoInfo(RTK_FIFO_H hFifo, ULONG* lpulFifoFree, ULONG* lpulFifoUsed);

#ifdef __cplusplus
}
#endif

#endif