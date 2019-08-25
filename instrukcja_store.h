#ifndef INSTRUKCJA_STORE_H
#define INSTRUKCJA_STORE_H

#include "cpu.h"
#include "ram.h"
#include "instrukcja.h"

class InstrukcjaStore : public Instrukcja {

	public:

		InstrukcjaStore(memory_type adr, register_type r) : adr(adr), r(r) { }

		void execute(CPU& cpu, RAM& ram) const {
			ram.setValue(adr, cpu.getValue(r));
		}

	private:
		memory_type adr;
		register_type r;
};

#endif // INSTRUKCJA_STORE_H
