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
    GPSTASK_PRTY = 10,

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

//extern Q     data_queue[]; // data queue, in this case array of Q-structs
//extern void* os_queue[];   // pointer queue for OS

//Functieprototypes voor functies binnen de main.
extern void OSinfo (void);
void InitProgram(void);
void MakeHandles();
void InitBoard(void);

// function prototypes of threads/tasks/processes to prevent compiler warnings
extern void GPSTask(void *pdata);

