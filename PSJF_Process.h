#ifndef PSJF_PROCESS_H
#define PSJF_PROCESS_H

#define STATE_WAITING = 0
#define STATE_READY = 1
#define STATE_RUNNING = 2
#define STATE_TERMINATED = 3

struct psjf_process {
    long pid,
    int hasRan,
    int state
};

#endif
