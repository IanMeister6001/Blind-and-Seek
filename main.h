//////////////////////////////////////////////////////////////////////////////
// prog: main.h
// comm: external definitions and prototypes for main
// auth: MSC
//////////////////////////////////////////////////////////////////////////////

// debug output stuff
#define OUTPUT_DEBUG

extern int Uart_debug_out; // bitmask-toggle key for task-debug-output
#define MUTEX_DEBUG_OUT   	0x01 // bit 1 used to toggle mutex-task output
#define MAILBOX_DEBUG_OUT 	0x02 // bit 2 used to toggle mbox-task output
#define FLAG_DEBUG_OUT    	0x04 // bit 3 used to toggle flag-task output
#define QUEUE_DEBUG_OUT   	0x08 // bit 4 used to toggle queue-task output
#define INTERRUPT_DEBUG_OUT 0x10 // bit 5 used to toggle interrupt-task output
#define STUDENTSTUFF_DEBUG_OUT 0x20 // bit 6 used to toggle studentstuff output

#define WAIT_FOREVER        0   // to force all OSPends (flag, mutex, mbox etc.) to wait for incoming signal
#define LOOP_DELAY          30  // standard delay time for tasks
#define LED_DELAY           50  // to slow leds down
#define QSIZE               10  // 10 q-members


// priorities for all tasks and prty of mutex-semaphore itself
enum {
    INITTASK_PRTY = 5,

    MUTEX_PRTY = 8,       // careful, this one should be highest of all using tasks

    GPSTASK_PRTY = 10,
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
extern OS_EVENT    *MutexHandle;
extern OS_EVENT    *MboxHandle;
extern OS_EVENT    *StudentstuffHandle;
extern OS_FLAG_GRP *FlagHandle;
extern OS_FLAG_GRP *FlagIntHandle;
extern OS_EVENT    *QueueHandle;

// q structure
typedef struct queue
{
 	int   nr;        // counter
	char  text[17];  // text to display
} Q, *PQ;

extern Q     data_queue[]; // data queue, in this case array of Q-structs
extern void* os_queue[];   // pointer queue for OS


// function prototypes for various tasks to prevent compiler warnings
extern void InitTask      (void *pdata);
extern void InitMutex     (void);
extern void DisplayOSData (void);
extern void CreateHandles (void);
extern void displayAllStackData(void);

// function prototypes of threads/tasks/processes to prevent compiler warnings
extern void MijnTask1     (void *pdata);
extern void GPSTask		  (void *pdata);
extern char * findToken(char buffer[], int tokenNr);

//Function prototypes of lorafunctions to prevent compiler warnings?
//extern char* sendrecv(char*);

