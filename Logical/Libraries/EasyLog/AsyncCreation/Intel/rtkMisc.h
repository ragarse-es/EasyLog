/**
 * @file
 * @brief Miscellaneous RTK functions
 * @ingroup rtk
 */

#ifndef _RTK_MISC_H_
#define _RTK_MISC_H_

#include <rtkBase.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Gets the current value of a microsecond counter.
 * 
 * Can be used for time measurements.
 * Overflow from 0xFFFFFFFF to 0x00000000 */
EXIMPORT ULONG RtkReadUsecCounter(void);

/**
 * @brief Gets the configured system tick */
EXIMPORT ULONG RtkGetSystemTick(void);

#ifdef __cplusplus
}
#endif

#endif
