/**
 * @file
 * @brief Contains legacy type definitions.
 * @note These types should only be used for calling functions which require these types.
 * @ingroup rtk
 */

#ifndef _RTK_TYPES_H_
#define _RTK_TYPES_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif

/** @cond */
#ifndef _ULONG_DEFINED_ /* ULONG might also be defined by vxTypesOld.h */
#define _ULONG_DEFINED_
/** @endcond */
typedef uint32_t ULONG;
#endif

/** @cond */
#ifndef _BYTE_DEFINED_ /* BYTE might already be defined from plctypes.h */
#define _BYTE_DEFINED_
/** @endcond */
typedef uint8_t BYTE;
#endif

/** @cond */
#ifndef _WORD_DEFINED_ /* WORD might already be defined from plctypes.h */
#define _WORD_DEFINED_
/** @endcond */
typedef uint16_t WORD;
#endif

typedef unsigned char BOOLEAN;
typedef char* LPSTRING;
typedef void* LPVOID;
typedef BYTE* LPBYTE;
typedef WORD* LPWORD;
typedef ULONG* LPULONG;

/** @brief RTK type of realtime clock */
typedef struct
{
	WORD Year;        /**< @brief 0 up (for example 1999)		*/
	BYTE Month;       /**< @brief 1-12							*/
	BYTE Day;         /**< @brief 1-31							*/
	BYTE DayOfWeek;   /**< @brief 1=monday, ..., 7=sunday		*/
	BYTE Hour;        /**< @brief 0-23							*/
	BYTE Minute;      /**< @brief 0-59							*/
	BYTE Second;      /**< @brief 0-59							*/
	WORD Millisecond; /**< @brief 0-999							*/
	WORD Microsecond; /**< @brief 0-999							*/
} RTK_RTC, *LPRTK_RTC;

#ifdef __cplusplus
}
#endif

#endif
