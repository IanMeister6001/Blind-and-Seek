//////////////////////////////////////////////////////////////////////////////
// prog: taskcreate.h
// comm: external definitions and prototypes for tasks
// auth: MSC
//////////////////////////////////////////////////////////////////////////////

#define STACK_SIZE               1000

// structure to simplify the creation of tasks
typedef struct TaskData
{
	void           (*taskname)(void*); // pointer to function
	OS_STK         *stack;             // stackaddress
	INT8U		    priority;          // priority = task-id
	INT32U		    stacksize;         // stacksize
	INT16U          option;            // to enable stackchecking
} TASKDATA, *PTASKDATA;	// name of struct, and pointer to struct

extern void CreateTasks(void);
extern void displayAllStackData(void);
extern void displayStackData(int);
