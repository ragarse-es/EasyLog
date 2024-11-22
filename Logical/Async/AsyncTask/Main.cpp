
#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
#include <AsDefault.h>
#endif
#include "rtk.h"


RTK_TASK_H TaskHandler1;
RTK_TASK_H TaskHandler2;

// amount of memory to be allocated for heap storage must be specified for every ANSI C++ program with the bur_heap_size variable
unsigned long bur_heap_size = 0xFFFF; 

//Function to execute asyncronously to increase the value of the counter
void IncreaseCounter(void *Params)
{
	counter++;
	//Delete the task once finished
	RtkDeleteTask(RtkGetCurrentTaskIdent());
}


void _INIT ProgramInit(void)
{
	// Insert code here 

}

void _CYCLIC ProgramCyclic(void)
{
	if (equalExecutes)
	{
		Execute2 = Execute1;
	}
	// Insert code here 
	//Launch the asyncronous task to create the logger
	if (Execute1)
	{	
		Execute1 = 0;
		LPRTK_CREATE_TASK_FKT function = &IncreaseCounter;
		RtkCreateTask(nullptr, 10, 0x1000, 0x1000, RTK_TASK_RESUMED, function, nullptr, &TaskHandler1); 
	}
	
	if (Execute2)
	{	
		Execute2 = 0;
		LPRTK_CREATE_TASK_FKT function = &IncreaseCounter;
		RtkCreateTask(nullptr, 10, 0x1000, 0x1000, RTK_TASK_RESUMED, function, nullptr, &TaskHandler2); 
	}

}

void _EXIT ProgramExit(void)
{
	// Insert code here 

}


