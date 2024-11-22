/**
 * @file
 * @brief RTK interrupt functions
 * @ingroup rtk
 * @note Do not use this interface! 
 */

#ifndef _RTK_INTERRUPT_H_
#define _RTK_INTERRUPT_H_

#include <arexport.h>

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Disables all interrupts */
AR_NOT_RECOMMENDED void RtkDisableInterrupts(void);

/** @brief Re-enables interrupts which were disabled by calling RtkDisableInterrupts */
AR_NOT_RECOMMENDED void RtkEnableInterrupts(void);

#ifdef __cplusplus
}
#endif

#endif
