//////////////////////////////////////////////////////////////////////////////
// prog: main.h
// comm: external definitions and prototypes for main
// auth: Projectgroep 2, EV3A
//////////////////////////////////////////////////////////////////////////////

#include <includes.h>

//Debugtoggle at compile time.
#define OUTPUT_DEBUG
#define NOODKNOP_ON			6	//Waarde die nodig is om te controleren of de noodknop is ingedrukt.
#define	TIPKNOP_ON			85  //Waarde die nodig is om te controleren of de tipknop is ingedrukt.
#define WAIT_FOREVER        0   // to force all OSPends (flag, mutex, mbox etc.) to wait for incoming signal
#define LOOP_DELAY          30  // standard delay time for tasks


// priorities for all tasks and prty of mutex-semaphore itself
enum {
    INITTASK_PRTY = 5,

    MUTEX_PRTY = 8,       // careful, this one should be highest of all using tasks

    KNOPTASK_PRTY = 10,
    LORATASK_PRTY,
    GPSTASK_PRTY,
    GAMETASK_PRTY,

    DUMMY
};


extern OS_EVENT *GPSDataHandle; //Handle voor de mbox tussen GPS-task en gametask.
extern OS_EVENT *MessageHandle; //Handle voor de mbox tussen gametask en TTN-task.
extern OS_FLAG_GRP *KnopFlag;   //Handle voor de flag tussen knoptask en gametask.


//Functieprototypes voor functies binnen de main.
extern void OSinfo (void);
void InitProgram(void);
void MakeHandles();
void InitBoard(void);
extern void BuildMessage(INT8U,int,int);
extern void SendZoekLocatie();

// function prototypes of threads/tasks/processes to prevent compiler warnings
extern void GPSTask(void *pdata);
extern void KnopTask(void *pdata);
extern void GameTask(void *pdata);
extern void LORATask(void *pdata);

