/* Zadanie 6.
 * Tomasz Kaszlewicz (tk334580) + Aleksander Matusiak (am347171)
 *
 * CPU
 */


#ifndef CPU_H
#define CPU_H

#include <stdint.h>
#include "exceptions.h"

using register_type = int;

class CPU {

	public:

		

		~CPU() {
			delete [] registers;
		}

		void setSize(int N) {
			if (N <= 0)
				throw IllegalArgumentException{};
			delete [] registers;
			size = N;
			registers = new int32_t[N];
		}

		register_type getSize() {
			return size;
		}

		void clean() {
			for (register_type i = 0; i < size; ++i)
				registers[i] = 0;
		}
		
		int32_t getValue(register_type r) const {
			return registers[r];
		}

		void setValue(register_type r, int32_t val) {
			registers[r] = val;
		}

	private:
		register_type size = 0; // potrzebne?
		int32_t *registers = nullptr;

};

#endif // CPU_H
