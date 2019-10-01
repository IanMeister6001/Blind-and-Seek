//////////////////////////////////////////////////////////////////////////////
// prog: taskcreate.c
// comm: task creation and stuff: OSTaskCreateExt, OSTaskStkChk
// auth: MSC
//////////////////////////////////////////////////////////////////////////////

#include "includes.h"
#include "taskcreate.h"
#include "main.h"


// Create different stacks for the tasks:
// give each thread/task/process its own stack with size
// stacks are allocated here statically, because malloc() is not supported
OS_STK MijnTask1_Stk     [STACK_SIZE];
OS_STK MutexTask1_Stk    [STACK_SIZE];
OS_STK MutexTask2_Stk    [STACK_SIZE];
OS_STK MutexTask3_Stk    [STACK_SIZE];
OS_STK MutexTask4_Stk    [STACK_SIZE];
OS_STK MboxPostTask_Stk  [STACK_SIZE];
OS_STK MboxPendTask_Stk  [STACK_SIZE];
OS_STK FlagPostTask_Stk  [STACK_SIZE];
OS_STK FlagPendTask_Stk  [STACK_SIZE];
OS_STK QueuePostTask_Stk [STACK_SIZE];
OS_STK QueuePendTask_Stk [STACK_SIZE];
OS_STK InterruptTask_Stk [STACK_SIZE];


// allocation of array of structures
// notes: the last structure is set to zero's, not necessary, but it simplifies
//        to disable a task: insert // in front of the line
// 1. looping thru the array (until null) and 2. copy-pasting with the last comma
TASKDATA tasks[] =
{
//  name           stack              priority            stacksize   option
// -------------------------------------------------------------------------------------
	// in studentstuff.c
//{   MijnTask1,     MijnTask1_Stk,     MIJNTASK1_PRTY,     STACK_SIZE, OS_TASK_OPT_STK_CHK  },
    // in mutex.c
//{   MutexTask1,    MutexTask1_Stk,    MUTEXTASK1_PRTY,    STACK_SIZE, OS_TASK_OPT_STK_CHK  },
//{   MutexTask2,    MutexTask2_Stk,    MUTEXTASK2_PRTY,    STACK_SIZE, OS_TASK_OPT_STK_CHK  },
//{   MutexTask3,    MutexTask3_Stk,    MUTEXTASK3_PRTY,    STACK_SIZE, OS_TASK_OPT_STK_CHK  },
//{   MutexTask4,    MutexTask4_Stk,    MUTEXTASK4_PRTY,    STACK_SIZE, OS_TASK_OPT_STK_CHK  },

    // in mailbox.c
{   MboxPostTask,  MboxPostTask_Stk,  MBOXPOSTTASK_PRTY,  STACK_SIZE, OS_TASK_OPT_STK_CHK  },
{   MboxPendTask,  MboxPendTask_Stk,  MBOXPENDTASK_PRTY,  STACK_SIZE, OS_TASK_OPT_STK_CHK  },

    // in flag.c
{   FlagPostTask,  FlagPostTask_Stk,  FLAGPOSTTASK_PRTY,  STACK_SIZE, OS_TASK_OPT_STK_CHK  },
{   FlagPendTask,  FlagPendTask_Stk,  FLAGPENDTASK_PRTY,  STACK_SIZE, OS_TASK_OPT_STK_CHK  },

    // in queue.c
{   QueuePostTask, QueuePostTask_Stk, QUEUEPOSTTASK_PRTY, STACK_SIZE, OS_TASK_OPT_STK_CHK  },
{   QueuePendTask, QueuePendTask_Stk, QUEUEPENDTASK_PRTY, STACK_SIZE, OS_TASK_OPT_STK_CHK  },

// in interrupt.c
{   InterruptTask, InterruptTask_Stk, INTERRUPTTASK_PRTY, STACK_SIZE, OS_TASK_OPT_STK_CHK  },

    // laatste, niet wissen: nodig om for-loop af te sluiten
{   NULL,          NULL,              0,                  0,          0                    }
};


//////////////////////////////////////////////////////////////////////////////
// func: createTasks
// args: none
// comm: all tasks are created in a loop, using the TASKDATA array
// note: the stacks are allocated statically; dynamic allocation would be an
//       improvement, but malloc() is not supported.
//////////////////////////////////////////////////////////////////////////////
void CreateTasks()
{
	// using a pointer to a structure: pointer is incremented
	PTASKDATA ptd = tasks; // tasks == &tasks[0]: both are addresses of first struct
	for (; ptd->taskname != NULL; ptd++)
	{
		OSTaskCreateExt(
			ptd->taskname,	 					// taskname
			NULL, 								// not used
			ptd->stack + ptd->stacksize-1, 		// top of stack
			ptd->priority,						// priority
            ptd->priority, 						// not used
			ptd->stack, 						// bottom of stack
			ptd->stacksize, 					// size of stack
			NULL, 								// not used
			ptd->option);						// enable stackchecking
	}
	// or, by using an integer to get the right structure
	// this version is simpler to understand, but less elegant
	/*
	//int       nr_of_tasks = tasks/sizeof(TASKDATA) -1; // nice, but not used
	int i;
	for (i=0; tasks[i].taskname != NULL; i++)
	{
		OSTaskCreateExt(
			tasks[i].taskname, 					// taskname
			NULL, 								// not used
			&tasks[i].stack[tasks[i].stacksize-1], 	// top of stack
			tasks[i].priority,					// priority
            tasks[i].priority, 					// not used
			tasks[i].stack, 					// bottom of stack
			tasks[i].stacksize, 				// size of stack
			NULL, 								// not used
			tasks[i].option);					// enable stackchecking
	}
	*/
}


//////////////////////////////////////////////////////////////////////////////
// func: displayStackData
// args: priority (== process-id)
// comm: functions retrieves and displays StackData-information
//       the os-call is optional and set in the STACKDATA array
// note: function is important, to check if task has enough memory, or
//       if task has too much memory
//////////////////////////////////////////////////////////////////////////////
void displayStackData(int priority)
{
    OS_STK_DATA stackdata; // allocate structure
	INT8U       error;

	if ((error = OSTaskStkChk(priority, &stackdata))) // get data onto this struct
	    return;

    UART_puts("\n\rStackData for process "); UART_putint(priority);
    UART_puts(", Usage=");     UART_putint((int)stackdata.OSUsed/4);
    UART_puts(" - Free=");	 UART_putint((int)stackdata.OSFree/4);
}


void displayAllStackData()
{
	PTASKDATA ptd = tasks; // tasks == &tasks[0]: both are addresses of first struct
	for (; ptd->taskname != NULL; ptd++)
		displayStackData(ptd->priority);
}
