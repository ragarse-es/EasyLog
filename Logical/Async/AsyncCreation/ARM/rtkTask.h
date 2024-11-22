/**
 * @file
 * @brief RTK task functions
 * @ingroup rtk
 */

#ifndef _RTK_TASK_H_
#define _RTK_TASK_H_

#include <rtkBase.h>

#ifdef __cplusplus
extern "C"
{
#endif

/** @brief RTK task handle */
typedef RTK_H RTK_TASK_H;

/** @brief RTK task timer handle */
typedef RTK_H RTK_TASK_TIMER_H;

/** @name Miscellaneous task macros */
/*@{*/
#define RTK_TASKIDENT_ITSELF 0x00000000
#define RTK_MAX_TASK_PRIORITY 255
#define RTK_MIN_TASK_PRIORITY 1
#define RTK_IDLE_TASK_PRIORITY 0x1000
#define RTK_MAX_TASK_VALUES 16
/*@}*/

/** @brief Type of task flags */
typedef unsigned long RTK_TASKFLAG;

/** @name Task flags (create/start) */
/*@{*/
#define RTK_TASK_RESUMED 0x00000000 /**< @brief task starts immediately */
#define RTK_TASK_SUSPENDED 0x00000001 /**< @brief task does not start immediately (to be started with RtkTaskResume()) */
#define RTK_TASK_SYSTEM 0x00000000 /**< @brief ignored */
#define RTK_TASK_DRIVER 0x00000002 /**< @brief ignored */
#define RTK_TASK_APPLICATION 0x00000004 /**< @brief ignored */
/*@}*/

/** @name Priority classes
 *
 * We use priority classes to define the real-time requirements of a task.
 * A task of a certain class will not be interrupted by tasks of a lower class.
 * But the task may be interrupted by other tasks of the same class,
 * so that all tasks of the same class get evenly access to a CPU. */
/*@{*/

/**
 * @brief A task of this class has real-time requirements.
 *
 * This class should be chosen,
 * if the task generates a response to an other system. A task of this class is expected
 * to wake up at most every 1ms and consumes very little CPU Power.
 */
#define RTK_TASK_REALTIME_1MS 0x00000008

/**
 * @brief A task of this class has some kind of real-time requirements.
 *
 * This class should be chosen, if the task generates a response to a user or to an other system.
 * A task of this class is expected to wake up at most every 10ms and consumes little CPU Power.
 */
#define RTK_TASK_REALTIME_10MS 0x00000010

/**
 * @brief A task of this class has no real-time requirement.
 *
 * This class should be chosen, if some background calculations are performed.
 */
#define RTK_TASK_BEST_EFFORT 0x00000020
/*@}*/

/** @brief Type of task event flags */
typedef unsigned long RTK_EVENTFLAG;

/** @name Task event flags */
/*@{*/
#define RTK_EVENT_ANY 0x00000000       /**< @brief wait until any of the wanted events are set */
#define RTK_EVENT_ALL 0x00000001       /**< @brief wait until all wanted events are set */
#define RTK_EVENT_NO_DELETE 0x00000002 /**< @brief ignored */
/*@}*/

/** @brief Type of task state flags */
typedef unsigned long RTK_TASKSTATEFLAG;

/** @name Task state flags (taskInfo)
 * | combination of task state flags | |
 * |:-|:-
 * |(RTK_TASKSTATE_DELAYED + RTK_TASKSTATE_SUSPENDED)| Task is both delayed and suspended
 * |(RTK_TASKSTATE_PENDED  + RTK_TASKSTATE_SUSPENDED)| Task is both pended and suspended */
/*@{*/
#define RTK_TASKSTATE_READY 0x00000000     /**< @brief Task is not waiting for any resource other than the CPU. */
#define RTK_TASKSTATE_SUSPENDED 0x00000001 /**< @brief Task is unavailable for execution. */
#define RTK_TASKSTATE_PENDED 0x00000002    /**< @brief Task is blocked due to the unavailability of some resource. */
#define RTK_TASKSTATE_DELAYED 0x00000004   /**< @brief Task is asleep for some duration. */
/*@}*/

/**
 * @brief Creates a task running function 'taskFunction'('taskFunctionParameter').
 *
 * The task has the name 'lpszTaskName' (must be unique or NULL, please also note the behaviour if the maximum length
 * #MAX_RTK_NAME_LENGTH is exceeded), the priority 'wTaskPriority' (1.. lowest, 255.. highest), and the stack size
 * 'ulTaskUserStackSize' + 'ulTaskSupervisorStackSize' in bytes. This combined stack size has to be greater or equal to
 * 0x1000 and less than or equal to 0x100000, otherwise the error #ERR_RTK_NO_STACK will be returned. The 'TaskFlags'
 * can be used to determine the initial task state (resumed/suspended). The function returns a handle to the created
 * task ('hTask') */
EXIMPORT RTK_ERROR RtkCreateTask(const char* lpszTaskName, WORD wTaskPriority, ULONG ulTaskSupervisorStackSize,
                                 ULONG ulTaskUserStackSize, RTK_TASKFLAG TaskFlags, LPRTK_CREATE_TASK_FKT taskFunction,
                                 void* taskFunctionParameter, RTK_TASK_H* hTask);

/**
 * @brief Deletes the task.
 *
 * Stops execution and frees stack of the task. */
EXIMPORT RTK_ERROR RtkDeleteTask(RTK_TASK_H hTask);

/**
 * @brief Suspends the task.
 *
 * A suspended task stops its execution until it is resumed (RtkResumeTask()) */
EXIMPORT RTK_ERROR RtkSuspendTask(RTK_TASK_H hTask);

/** @brief Resumes the task. */
EXIMPORT RTK_ERROR RtkResumeTask(RTK_TASK_H hTask);

/** @brief Delays the calling task for the specified time in us. */
EXIMPORT void RtkSleepTaskUsec(ULONG ulSleepUsec);

/**
 * @brief Sends an event to a task ('hTask').
 *
 * Each bit of 'wTaskEvent' is one event of the task - sending multiple events is possible (logical OR of event-bits).
 * Sending one event-bit multiple times (e.g. by calling this function twice) has no effect if the event was not yet
 * read with RtkReceiveTaskEvent(). */
EXIMPORT RTK_ERROR RtkSendTaskEvent(RTK_TASK_H hTask, WORD wTaskEvents);

/**
 * @brief Waits for one ('EventFlags'=#RTK_EVENT_ANY) or all ('EventFlag'=#RTK_EVENT_ALL) of the requested
 * Events and returns these events. Resets all returned events of the task.
 * @param hTask            Handle of the task for which the events shall be returned
 * @param wWantedEvents    Bit-mask of the events which shall be returned
 * @param EventFlags       With 'EventFlags'=#RTK_EVENT_ANY this function returns if any of the events specified with
 *                         'wWantedEvents' is received. With 'EventFlags'=#RTK_EVENT_ALL this function only returns if
 * all of the events specified with 'wWantedEvents' is received.
 * @param ulTimeoutUsec    If the specified timeout is over before the events are received, the function returns with an
 *                         error and 'lpwReceivedEvent' is not valid. If some of the requested events were already
 * received by the task, then these events are not reset.
 * @param lpwReceivedEvent This output-parameter contains the received events after the function call.
 *                         Parameter must not be NULL. */
EXIMPORT RTK_ERROR RtkReceiveTaskEvent(RTK_TASK_H hTask, WORD wWantedEvents, RTK_EVENTFLAG EventFlags,
                                       ULONG ulTimeoutUsec, WORD* lpwReceivedEvents);

/**
 * @brief Cyclically sends events to a task ('hTask').
 *
 * The events will be sent every 'ulDelayUsec' us.
 * Each bit of 'wTaskEvent' is one event of the task - sending multiple events is possible (logical OR of event-bits)
 * Sending one event-bit multiple times has no effect (e.g. if the timer expires and the event is re-sent before the
 * event was read with RtkReceiveTaskEvent()).
 * Returns a timer handle ('hTaskTimer' - must not be NULL) which can be used to stop sending the events
 * (RtkCancelTaskTimer()) */
EXIMPORT RTK_ERROR RtkSendTaskEventCyclic(RTK_TASK_H hTask, WORD wTaskEvents, ULONG ulDelayUsec,
                                          RTK_TASK_TIMER_H* hTaskTimer);

/**
 * @brief Sends events once to a task ('hTask').
 *
 * The events will be sent after 'ulDelayUsec' us.
 * Each bit of 'wTaskEvent' is one event of the task - sending multiple events is possible (logical OR of event-bits).
 * Sending one event-bit multiple times (e.g. by calling this function twice) has no effect if the event was not yet
 * read with RtkReceiveTaskEvent().
 * Returns a timer handle ('hTaskTimer' - must not be NULL) which can be used to stop sending the event
 * (RtkCancelTaskTimer()) */
EXIMPORT RTK_ERROR RtkSendTaskEventAfter(RTK_TASK_H hTask, WORD wTaskEvents, ULONG ulDelayUsec,
                                         RTK_TASK_TIMER_H* hTaskTimer);

/**
 * @brief Cancels the provided timer ('hTaskTimer').
 *
 * This results in canceling sending the events initiated with RtkSendTaskEventCyclic() or RtkSendTaskEventAfter() */
EXIMPORT RTK_ERROR RtkCancelTaskTimer(RTK_TASK_TIMER_H hTaskTimer);

/**
 * @brief Writes to one of the 16 available local variables of the task ('hTask').
 *
 * 'byValueIndex' (0-15) addresses the variables, 'ulValue' is the values to be written */
EXIMPORT RTK_ERROR RtkSetTaskValue(RTK_TASK_H hTask, BYTE byValueIndex, ULONG ulValue);

/**
 * @brief Reads to one of the 16 available local variables of the task ('hTask').
 *
 * 'byValueIndex' (0-15) addresses the variables, 'lpulValue' contains the value of the variable after the function call
 */
EXIMPORT RTK_ERROR RtkGetTaskValue(RTK_TASK_H hTask, BYTE byValueIndex, ULONG* lpulValue);

/** @brief Sets the task priority to 'wTaskPriority' (1.. lowest, 255.. highest) */
EXIMPORT RTK_ERROR RtkSetTaskPriority(RTK_TASK_H hTask, WORD wTaskPriority);

/**
 * @brief Retrieves the task priority.
 *
 * 'lpwTaskPriority' contains the priority of the task after the function call */
EXIMPORT RTK_ERROR RtkGetTaskPriority(RTK_TASK_H hTask, WORD* lpwTaskPriority);

/**
 * @brief Returns task information.
 *
 * @param hTask              Handle of the task for which the information shall be returned
 * @param lpTaskStateFlags   Output-parameter - can be NULL. Returns the task state (RTK_TASKSTATE_READY,
 *                           RTK_TASKSTATE_SUSPENDED, RTK_TASKSTATE_PENDED, RTK_TASKSTATE_DELAYED)
 * @param lpwTaskPriority    Output-parameter - can be NULL. Returns the task priority
 * @param lpulUnused1        unused - can be NULL
 * @param lpulUnused2        unused - can be NULL
 * @param lpulFreeStack      Output-parameter - can be NULL. Returns the free stack size
 * @param pInternalTaskIdent Output-parameter - can be NULL. Returns an OS-specific task ident */
EXIMPORT RTK_ERROR RtkGetTaskInfo(RTK_TASK_H hTask, RTK_TASKSTATEFLAG* lpTaskStateFlags, WORD* lpwTaskPriority,
                                  ULONG* lpulUnused1, ULONG* lpulUnused2, ULONG* lpulFreeStack,
                                  void** pInternalTaskIdent);

/** @brief Get the handle for the currently running task */
EXIMPORT RTK_TASK_H RtkGetCurrentTaskIdent(void);

/** @returns TRUE if the task is ready, FALSE if the task is not ready or if 'hTask' is invalid. */
EXIMPORT BOOLEAN RtkIsTaskReady(RTK_TASK_H hTask);

/** @returns TRUE if the task is suspended, FALSE if the task is not suspended or if 'hTask' is invalid. */
EXIMPORT BOOLEAN RtkIsTaskSuspended(RTK_TASK_H hTask);

#ifdef __cplusplus
}
#endif

#endif
