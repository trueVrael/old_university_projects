#ifndef INSTRUKCJA_SET_H
#define INSTRUKCJA_SET_H

#include "cpu.h"
#include "ram.h"
#include "instrukcja.h"

class InstrukcjaSet : public Instrukcja {

	public:

		InstrukcjaSet(register_type r, int32_t val) : r(r), val(val) { }

		void execute(CPU& cpu, RAM&) const {
			cpu.setValue(r, val);
		}

	private:
		register_type r;
		int32_t val;
		
};

#endif // INSTRUKCJA_SET_H
