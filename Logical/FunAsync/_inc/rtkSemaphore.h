/**
 * @file
 * @brief RTK semaphore functions
 * 
 * Semaphores can be used to control access to critical code sections or to synchronize tasks.
 * 
 * @ingroup rtk
 */

#ifndef _RTK_SEMAPHORE_H_
#define _RTK_SEMAPHORE_H_

#include <rtkBase.h>

#ifdef __cplusplus
extern "C" {
#endif

/** @brief RTK semaphore handle */
typedef RTK_H RTK_SEM_H;

/**
 * @brief Creates a semaphore with the provided name ('lpszSemaphoreName' must be unique or NULL).
 * 
 * 'ulInitCount' defines the initial value of the semaphore (RtkAcquireSemaphore() decreases this value until 0,
 * RtkReleaseSemaphore() increases this value until 'ulMaxCount').
 * The output-parameter 'hSemaphore' (must not be NULL) returns a handle to the semaphore */
EXIMPORT RTK_ERROR
RtkCreateSemaphoreEx(const char* lpszSemaphoreName, ULONG ulInitCount, ULONG ulMaxCount, RTK_SEM_H* hSemaphore);

/**
 * @brief Creates a semaphore with the provided name ('lpszSemaphoreName' must be unique or NULL).
 * 
 * The initial value of the semaphore is 'ulMaxCount' (RtkAcquireSemaphore() decreases this value until 0,
 * RtkReleaseSemaphore() increases this value until 'ulMaxCount').
 * The output-parameter 'hSemaphore' (must not be NULL) returns a handle to the semaphore */
EXIMPORT RTK_ERROR RtkCreateSemaphore(const char* lpszSemaphoreName, ULONG ulMaxCount, RTK_SEM_H* hSemaphore);

/**
 * @brief Deletes a sempahore */
EXIMPORT RTK_ERROR RtkDeleteSemaphore(RTK_SEM_H hSemaphore);

/**
 * @brief Blocks until the semaphore has a value higher than 0 or until the timeout expires.
 *
 * When the semaphore has a value higher than 0, then this function decrements this value and returns. */
EXIMPORT RTK_ERROR RtkAcquireSemaphore(RTK_SEM_H hSemaphore, ULONG ulTimeoutUsec);

/** @brief Increment the value of the semaphore.
 * 
 *  Only if the value was lower than the 'ulMaxCount' defined when creating the semaphore. */
EXIMPORT RTK_ERROR RtkReleaseSemaphore(RTK_SEM_H hSemaphore);

/**
 * @brief Obtains information about the semaphore.
 * @param lpulMaxCount         Output-parameter - can be NULL. Returns ulMaxCount defined when creating the semaphore
 * @param lpulActCount         Output-parameter - can be NULL. Returns the current value of the semaphore
 * @param lpulTaskWaitingCount Output-parameter - can be NULL. Returns the number of tasks waiting for this semaphore */
EXIMPORT RTK_ERROR
RtkGetSemaphoreInfo(RTK_SEM_H hSemaphore, ULONG* lpulMaxCount, ULONG* lpulActCount, ULONG* lpulTaskWaitingCount);

#ifdef __cplusplus
}
#endif

#endif
