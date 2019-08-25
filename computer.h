/* Zadanie 6.
 * Tomasz Kaszlewicz (tk334580) + Aleksander Matusiak (am347171)
 *
 * Computer
 */
 
#include <iostream>
#include <memory>

#include "cpu.h"
#include "ram.h"
#include "os.h"
#include "scheduling_algorithm.h"

#ifndef COMPUTER_H
#define COMPUTER_H

class Computer {

	public:

		void setCPU(register_type numOfRegisters) {
			cpu.setSize(numOfRegisters);
		}

		void setRAM(memory_type size) {
			ram.setSize(size);
		}

		std::shared_ptr<OS> installOS(std::shared_ptr<SchedulingAlgorithm> alg) {
			if (!cpu.getSize())
				throw NoCPUException{};
			if (!ram.getSize())
				throw NoRAMException{};
			
			cpu.clean();
			ram.clean();
			return std::make_shared<OS>(OS{cpu, ram, alg});
		}

	private:
		CPU cpu;
		RAM ram;


};



#endif // COMPUTER_H
