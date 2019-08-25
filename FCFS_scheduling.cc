#include "FCFS_scheduling.h"

FCFSScheduling::FCFSScheduling() {
}

FCFSScheduling::~FCFSScheduling() {
}

void FCFSScheduling::setProcesses(std::vector<std::shared_ptr<Proces> > v) {
    while (!processes_in_order.empty()){
        processes_in_order.pop();
    }
    for (auto proc_ptr : v)
        processes_in_order.push(proc_ptr);
}
//do wywalenia, al einaczej mi si� nie kompilowa�o
int main(){
	return 0;
}

