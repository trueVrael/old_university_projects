#ifndef INSTRUKCJA_H
#define INSTRUKCJA_H

#include "cpu.h"
#include "ram.h"
#include "instrukcja.h"

class Instrukcja {

	public:

		virtual void execute(CPU& cpu, RAM& ram) const = 0; // do zmiany na = 0

};

#endif // INSTRUKCJA_H
