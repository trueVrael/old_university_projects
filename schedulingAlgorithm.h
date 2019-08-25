#ifndef SCHEDULING_ALGORITHM
#define SCHEDULING_ALGORITHM

#include <vector>
#include "proces.h"

class SchedulingAlgorithm {

	public:
		std::pair<int, int> next() {
			return std::make_pair(-1,0);
		}

		virtual void setProcesses(std::vector<Proces> v) {

		}

};


#endif // SCHEDULING_ALGORITHM
