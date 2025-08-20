#include "OS.h"
#include <string.h>
#include <unistd.h>
#include <stdio.h>

int Os_CreateTask(OsTaskType* tcb, TaskEntryType entry, void* arg, int priority, const char* name)
{
    if (!tcb || !entry) return -1;
    tcb->entry = entry;
    tcb->arg = arg;
    tcb->priority = priority;
    if (name) snprintf(tcb->name, sizeof(tcb->name), "%s", name);
    return 0;
}

static void* os_task_entry(void* arg)
{
    OsTaskType* tcb = (OsTaskType*)arg;
    if (tcb->entry) tcb->entry(tcb->arg);
    return NULL;
}

int Os_StartTask(OsTaskType* tcb)
{
    if (!tcb) return -1;
    return pthread_create(&tcb->thread, NULL, os_task_entry, tcb);
}

void Os_Sleep(uint32_t ms)
{
    usleep(ms * 1000);
}

void Os_TerminateTask(void)
{
    pthread_exit(NULL);
}
