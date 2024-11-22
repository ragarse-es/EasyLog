/** 
 * @file
 * @brief This headerfile contains macros used to indicate the validity of exported functions.
 * @ingroup arsystem
 * 
 */

#ifndef _AR_EXPORT_H_
#define _AR_EXPORT_H_

/**
 * @brief Functions marked with #AR_DEPRECATED should not be used in AR-external code at all.
 * 
 * AR still supports these functions, but cannot provide guarantees for the compatibility
 * of these functions.
 * Any AR-external code using #AR_DEPRECATED functions should replace these functions with
 * the equivalent exported function which is not marked as #AR_DEPRECATED. 
 */
#if !defined(AR_CORE) && defined(__GNUC__)
#define AR_DEPRECATED __attribute__((deprecated))
#else
#define AR_DEPRECATED
#endif

/**
 * @brief Functions marked with #AR_NOT_RECOMMENDED should not be used when writing new AR-external code.
 * 
 * For these functions there is no functionally equivalent alternative available and thus
 * they cannot be replaced easily in existing code (that is why they are not #AR_DEPRECATED).
 */
#define AR_NOT_RECOMMENDED

#endif
