//////////////////////////////////////////////////////////////////////////////
// prog: main.h
// comm: external definitions and prototypes for main
// auth: Projectgroep 2, EV3A
//////////////////////////////////////////////////////////////////////////////

//Debugtoggle at compile time.
#define OUTPUT_DEBUG

#define WAIT_FOREVER        0   // to force all OSPends (flag, mutex, mbox etc.) to wait for incoming signal
#define LOOP_DELAY          30  // standard delay time for tasks


// priorities for all tasks and prty of mutex-semaphore itself
enum {
    INITTASK_PRTY = 5,

    MUTEX_PRTY = 8,       // careful, this one should be highest of all using tasks
    MUTEXTASK1_PRTY = 10,
    MUTEXTASK2_PRTY,
    MUTEXTASK3_PRTY,
    MUTEXTASK4_PRTY,

    MIJNTASK1_PRTY,
    MBOXPOSTTASK_PRTY,
    MBOXPENDTASK_PRTY,


    FLAGPOSTTASK_PRTY = 22,
    FLAGPENDTASK_PRTY,

    QUEUEPOSTTASK_PRTY,
    QUEUEPENDTASK_PRTY,

    INTERRUPTTASK_PRTY,

    DUMMY
};


// handles, used for semaphore, mailbox, flag, queue
// defined as a pointer; the os will allocate an OS_EVENT on it
/*extern OS_EVENT    *MutexHandle;
extern OS_EVENT    *MboxHandle;
extern OS_EVENT    *StudentstuffHandle;
extern OS_FLAG_GRP *FlagHandle;
extern OS_FLAG_GRP *FlagIntHandle;
extern OS_EVENT    *QueueHandle;*/

//Oude queue structuur. Blijf hier staan voor het geval we toch een queue nodig hebben zodat we dit als blauwdruk kunnen maken.
/*typedef struct queue
{
 	int   nr;        // counter
	char  text[17];  // text to display
} Q, *PQ;

extern Q     data_queue[]; // data queue, in this case array of Q-structs
extern void* os_queue[];   // pointer queue for OS
*/

//Functieprototypes voor functies binnen de main.
extern void OSinfo (void);
void InitProgram(void);
void MakeHandles();
void InitBoard(void);

// function prototypes of threads/tasks/processes to prevent compiler warnings
/*extern void MijnTask1     (void *pdata);
extern void MijnTask2     (void *pdata);
extern void Postmijntask  (void *pdata);
extern void MutexTask1    (void *pdata);
extern void MutexTask2    (void *pdata);
extern void MutexTask3    (void *pdata);
extern void MutexTask4    (void *pdata);
extern void MboxPostTask  (void *pdata);
extern void MboxPendTask  (void *pdata);
extern void FlagPostTask  (void *pdata);
extern void FlagPendTask  (void *pdata);
extern void QueuePostTask (void *pdata);
extern void QueuePendTask (void *pdata);
extern void InterruptTask (void *pdata);*/

