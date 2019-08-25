#ifndef FCFS_SCHEDULING_H
#define FCFS_SCHEDULING_H

#include <queue>
#include "scheduling_algorithm.h"

class FCFSScheduling : public SchedulingAlgorithm {
    std::queue<std::shared_ptr<Proces> > processes_in_order;
public:
    FCFSScheduling();
    virtual ~FCFSScheduling();
    virtual void setProcesses(std::vector< std::shared_ptr<Proces> > v);
};

#endif // FCFS_SCHEDULING_H
