#ifndef OS_H
#define OS_H

#include <stdint.h>
#include <pthread.h>

typedef void (*TaskEntryType)(void*);

typedef struct {
    pthread_t thread;
    TaskEntryType entry;
    void* arg;
    int priority;
    char name[32];
    int started;
} OsTaskType;

int Os_CreateTask(OsTaskType* tcb, TaskEntryType entry, void* arg, int priority, const char* name);
int Os_StartTask(OsTaskType* tcb);
void Os_Sleep(uint32_t ms);
void Os_TerminateTask(void);

#endif
