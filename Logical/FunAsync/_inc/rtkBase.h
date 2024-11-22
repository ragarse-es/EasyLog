/**
 * @file
 * @brief RTK basis data types and error codes
 * @ingroup rtk
 */

#ifndef _RTK_BASE_H_
#define _RTK_BASE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <rtkTypes.h>
#include <arexport.h>

/** @cond */

/* For using this file in Automation Studio tasks, it has to be included AFTER AsDefault.h */
#if defined (_DEFAULT_INCLUDES) && !defined(_RUNTIME_) && !defined(_REPLACE_CONST)
#error Rtk-headers have to be be included after AsDefault.h
#endif

/* COMPILER_SPECIFICS */
#ifndef DECLSPEC_NOTHROW
#if defined(__GNUC__) && ((__GNUC__ > 3) || ((__GNUC__ == 3) && (__GNUC_MINOR__ >= 3)))
#define DECLSPEC_NOTHROW __attribute__((__nothrow__))
#elif defined(__cplusplus) && (_MSC_VER >= 1200)
#define DECLSPEC_NOTHROW __declspec(nothrow)
#else
#define DECLSPEC_NOTHROW
#endif
#endif

/* Can be used to decorate RTK functions with additional attributes. */
#ifndef EXIMPORT
#if defined(_CPPUNWIND) || defined(__EXCEPTIONS)
#define EXIMPORT DECLSPEC_NOTHROW
#else
#define EXIMPORT
#endif
#endif

/** @endcond */

/** @name General definitions for RT kernel */
/*@{*/
/**
 * @brief Maximum length of an RTK object (task name, ...).
 * @note Longer names will be shortened to the maximum length! No error is reported. */
#define MAX_RTK_NAME_LENGTH 32
#define MAX_RTK_NAME_TERMINATED_LENGTH (MAX_RTK_NAME_LENGTH + 1)
/*@}*/

/** @name Timeout for task events, semaphore, fifo, debugmessage, ... */
/*@{*/
#define RTK_TIMEOUT_NOWAIT 0
#define RTK_TIMEOUT_WAITFOREVER 0xFFFFFFFF
/*@}*/

/** @brief RTK task function prototype */
typedef void (*LPRTK_CREATE_TASK_FKT)(void* param);

/** 
 * @brief ERR_OK could be defined by spserror.h or runtime.h
 * @note If a stdlib has spserror.h included, we have to return ERR_OK 0U because the stdlibs catch ERR_OK with 0U via runtime.h */
#ifndef ERR_OK
#define ERR_OK 0U
#endif

/** @brief RTK error codes */
/* 25100 - 25199 obsolete (Debugger error codes) */
typedef enum
{
	ERR_RTK_OK = ERR_OK,

	ERR_RTK_OBJID = 25000, /**< @brief Invalid object ident */
	ERR_RTK_OBJNOTFOUND,   /**< @brief Object with specified name not found */
	ERR_RTK_OBJTYPE,       /**< @brief Invalid object type */
	ERR_RTK_OBJ_OVERFLOW,  /**< @brief Object table full */
	ERR_RTK_OBJ_EXIST,     /**< @brief Object already exists */
	ERR_RTK_OBJNAME,       /**< @brief Wrong object name */

	ERR_RTK_PRIORITY = 25010, /**< @brief Invalid priority */
	ERR_RTK_NO_STACK,         /**< @brief Not enough free stack */
	ERR_RTK_NOT_SUSPENDED,    /**< @brief Task is not suspended */
	ERR_RTK_SUSPENDED,        /**< @brief Task is already suspended */
	ERR_RTK_TASKVALUE,        /**< @brief Invalid valueindex */
	ERR_RTK_NOTASKTIMERS,     /**< @brief No more task timers available */
	ERR_RTK_TASKCREATE,       /**< @brief OS task not created */
	ERR_RTK_TASKDELETE,       /**< @brief OS Task not deleted */

	ERR_RTK_FIFO_CREATE = 25030,   /**< @brief FIFO not created (memory alloc, ...) */
	ERR_RTK_FIFO_DELETE,           /**< @brief FIFO not deleted (memory free, ...) */
	ERR_RTK_FIFO_WRITE,            /**< @brief FIFO not written (fifo full, ...) */
	ERR_RTK_FIFO_READ,             /**< @brief FIFO not read (fifo empty, ...) */
	ERR_RTK_FIFO_RELEASE,          /**< @brief FIFO not released */
	ERR_RTK_FIFO_ATTACHED_TIMEOUT, /**< @brief FIFO read/write with timeout invalid (attached) */
	ERR_RTK_FIFO_NOT_ATTACHED,     /**< @brief FIFO not attached when read/write */
	ERR_RTK_FIFO_ATTACHED,         /**< @brief FIFO already attached (other task) */
	ERR_RTK_FIFO_ATTACH,           /**< @brief FIFO not created for attach */

	ERR_RTK_SEMACREATE = 25040, /**< @brief OS semaphore not created */
	ERR_RTK_SEMA_DELETE,        /**< @brief OS semaphore not deleted */
	ERR_RTK_SEMA_RELEASE,       /**< @brief OS semaphore not released */

	ERR_WRONG_MEMTYPE = 25060, /**< @brief Invalid memory type */
	ERR_RTK_NOMEM_HEAP,        /**< @brief Not enough memory free in heap */
	ERR_RTK_FREE_HEAP,         /**< @brief Memory not in heap */

	ERR_RTK_CREATE_SHAREDMEM, /**< @brief SharedMemory not created */
	ERR_RTK_DELETE_SHAREDMEM, /**< @brief SharedMemory not deleted */
	ERR_RTK_MAP_SHAREDMEM,    /**< @brief SharedMemory not mapped */
	ERR_RTK_UNMAP_SHAREDMEM,  /**< @brief SharedMemory not unmapped */

	ERR_RTK_IRQNUMBER = 25080, /**< @brief Invalid interruptnumber */
	ERR_RTK_CONNECT_IRQ,       /**< @brief Interrupt not connected */
	ERR_RTK_DISCONNECT_IRQ,    /**< @brief Interrupt not disconnected */

	ERR_RTK_TIMERCYCLE = 25090, /**< @brief Invalid cycletime */
	ERR_RTK_WRONGTIME,          /**< @brief Invalid time/date */
	ERR_RTK_SET_RTC,            /**< @brief RealTimeClock not set */
	ERR_RTK_TIMER_CREATE,       /**< @brief Timer not created */
	ERR_RTK_TIMER_DELETE,       /**< @brief Timer not deleted */
	ERR_RTK_TIMER_START,        /**< @brief Timer not started */
	ERR_RTK_TIMER_STOP,         /**< @brief Timer not stopped */

	ERR_RTK_SOFTPLC_CREATE = 25200, /**< @brief RtkCreateSoftPlc not possible */
	ERR_RTK_INIT,                   /**< @brief RTK not initialized */
	ERR_RTK_DEINIT,                 /**< @brief RTK not deinitialized */
	ERR_RTK_FATAL,                  /**< @brief Fatal internal RTK error */
	ERR_RTK_TIMEOUT,                /**< @brief Timeout in RTK functions with wait */
	ERR_RTK_NOTSUPPORTED,           /**< @brief Function not supported in this RTK version */
	ERR_RTK_UNHANDLED_CPP_EXC,      /**< @brief Unhandled CPP Exception */

	ERR_RTK_CRITSECCREATE = 25250, /**< @brief CriticalSection not created */
	ERR_RTK_CRITSECDELETE,         /**< @brief CriticalSection not deleted */
	ERR_RTK_CRITSECENTER,          /**< @brief CriticalSection not enterd */
	ERR_RTK_CRITSECLEAVE,          /**< @brief CriticalSection not left */

	ERR_RTK_EXC_BASE = 25300, /**< @brief Base for RTK exceptions -> + RTK_EXCTYP_xx */

	ERR_RTK_LOG_CREATE = 25400, /**< @brief Log not created */
	ERR_RTK_LOG_DELETE,         /**< @brief Log not deleted  */
	ERR_RTK_LOG_NOBUFSET,       /**< @brief no Log-Buffer set */
	ERR_RTK_LOG_LOGACTIVE,      /**< @brief Log active */
} RTK_ERROR;

/** @brief General RTK object handle */
typedef ULONG RTK_H;

#ifdef __cplusplus
}
#endif

#endif
