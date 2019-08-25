#ifndef INSTRUKCJA_LOAD_H
#define INSTRUKCJA_LOAD_H

#include "cpu.h"
#include "ram.h"
#include "instrukcja.h"

class InstrukcjaLoad : public Instrukcja {

	public:

		InstrukcjaLoad(register_type r, memory_type adr) : r(r), adr(adr) { }

		void execute(CPU& cpu, RAM& ram) const {
			cpu.setValue(r, ram.getValue(adr));
		}

	private:
		register_type r;
		memory_type adr;
		
};

#endif // INSTRUKCJA_LOAD_H
