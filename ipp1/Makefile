HEADERS = structure.h parse.h

default: hospital

structure.o: structure.c
	gcc -c structure.c -o structure.o

parse.o: parse.c
	gcc -c parse.c -o parse.o

hospital.o: hospital.c
	gcc -c hospital.c -o hospital.o

hospital: hospital.o structure.o parse.o
	gcc hospital.o structure.o parse.o -o hospital

debug: hospital.o structure.o parse.o
	gcc -g hospital.o structure.o parse.o -o hospital.dbg

clean:
	rm -rf ./*.o
	rm -rf ./hospital
	rm -rf ./hospital.dbg
