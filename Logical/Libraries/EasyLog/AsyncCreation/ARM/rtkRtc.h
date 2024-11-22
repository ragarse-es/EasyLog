/**
 * @file
 * @brief Functions to set and read the real-time-clock
 * @ingroup rtk
 */

#ifndef _RTK_RTC_H_
#define _RTK_RTC_H_

#include <rtkBase.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief  Set the real-time-clock to 'newDateTime'.
 * 
 *  'newDateTime' has to contain the localtime (not the UTC time). */
EXIMPORT RTK_ERROR RtkSetRealTimeClock(RTK_RTC const* newDateTime);

/** @brief Retrieves the current date and localtime. */
EXIMPORT void RtkGetRealTimeClock(RTK_RTC* dateTime);

#if AR_CORE
#include "private/rtkRtcP.h"
#endif

#ifdef __cplusplus
}
#endif

#endif
