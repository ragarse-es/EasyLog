
#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

#include <stdio.h>
#include "rtk.h"

void myAsynchronousFunction(void)
{
	do
	{
	  Counter++;
	} while (Block);
	RtkDeleteTask(RtkGetCurrentTaskIdent());
}


void _CYCLIC ProgramCyclic(void)
{
	if (Cmd[0])
	{
		Cmd[0] = 0;
		LPRTK_CREATE_TASK_FKT function = (LPRTK_CREATE_TASK_FKT)&myAsynchronousFunction;
		RTK_TASK_H hTask;
		RtkCreateTask(NULL, 50, 0x1000, 0x1000, 0, function, NULL, &hTask);	
	} else if (Cmd[1])
	{
		Cmd[1] = 0;
		LPRTK_CREATE_TASK_FKT function = (LPRTK_CREATE_TASK_FKT)&myAsynchronousFunction;
		RTK_TASK_H hTask0;
		RTK_TASK_H hTask1;
		RtkCreateTask(NULL, 50, 0x1000, 0x1000, 0, function, NULL, &hTask0);
		RtkCreateTask(NULL, 50, 0x1000, 0x1000, 0, function, NULL, &hTask1);
	}
}


