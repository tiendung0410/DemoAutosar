#include "OS.h"
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sched.h>

static void* os_task_entry(void* arg)
{
    OsTaskType* tcb = (OsTaskType*)arg;
    if (tcb->entry) tcb->entry(tcb->arg);
    return NULL;
}

int Os_CreateTask(OsTaskType* tcb, TaskEntryType entry, void* arg, int priority, const char* name)
{
    if (!tcb || !entry) return -1;
    memset(tcb, 0, sizeof(OsTaskType));
    tcb->entry = entry;
    tcb->arg = arg;
    tcb->priority = priority;
    tcb->started = 0;
    strncpy(tcb->name, name, sizeof(tcb->name)-1);
    return 0;
}

int Os_StartTask(OsTaskType* tcb)
{
    if (!tcb || tcb->started) return -1;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setschedpolicy(&attr, SCHED_FIFO);
    struct sched_param param = { .sched_priority = tcb->priority };
    pthread_attr_setschedparam(&attr, &param);
    pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);

    if (pthread_create(&tcb->thread, &attr, os_task_entry, tcb) != 0) {
        pthread_attr_destroy(&attr);
        return -2;
    }
    tcb->started = 1;
    pthread_attr_destroy(&attr);
    return 0;
}

void Os_Sleep(uint32_t ms)
{
    usleep(ms * 1000);
}

void Os_TerminateTask(void)
{
    pthread_exit(NULL);
}
