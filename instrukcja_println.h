#ifndef INSTRUKCJA_PRINTLN_H
#define INSTRUKCJA_PRINTLN_H

#include "cpu.h"
#include "ram.h"
#include "instrukcja.h"

#include <iostream>

class InstrukcjaPrintln : public Instrukcja {

	public:

		InstrukcjaPrintln(register_type r) : r(r) { }

		void execute(CPU& cpu, RAM&) const {
			std::cout << cpu.getValue(r) << "\n";
		}

	private:
		register_type r;
		
};

#endif // INSTRUKCJA_PRINTLN_H
