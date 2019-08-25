#ifndef INSTRUKCJA_SUB_H
#define INSTRUKCJA_SUB_H

#include "cpu.h"
#include "ram.h"
#include "instrukcja.h"

class InstrukcjaSub : public Instrukcja {

	public:

		InstrukcjaSub(register_type r1, register_type r2) : r1(r1), r2(r2) { }

		void execute(CPU& cpu, RAM&) const {
			int32_t new_value = cpu.getValue(r1) - cpu.getValue(r2);
			cpu.setValue(r1, new_value);
		}

	private:
		register_type r1;
		register_type r2;
};

#endif // INSTRUKCJA_SUB_H
