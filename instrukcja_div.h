#ifndef INSTRUKCJA_DIV_H
#define INSTRUKCJA_DIV_H

#include "cpu.h"
#include "ram.h"
#include "instrukcja.h"
#include "division_by_zero_exception.h"

class InstrukcjaDiv : public Instrukcja {

	public:

		InstrukcjaDiv(register_type r1, register_type r2) : r1(r1), r2(r2) { }

		void execute(CPU& cpu, RAM&) const {
			int val2 = cpu.getValue(r2);
			if (val2 == 0)
				throw DivisionByZeroException{};
			int32_t new_value = cpu.getValue(r1) / val2;
			cpu.setValue(r1, new_value);
		}

	private:
		register_type r1;
		register_type r2;
};

#endif // INSTRUKCJA_DIV_H
