all: gestore_aula.exe test_gestore.exe

# Creazione degli eseguibili finali
gestore_aula.exe: src/main.o src/aula.o src/studente.o src/coda.o src/prenotazione.o
	gcc -o gestore_aula.exe src/main.o src/aula.o src/studente.o src/coda.o src/prenotazione.o

test_gestore.exe: src/main_test.o src/aula.o src/studente.o src/coda.o src/prenotazione.o
	gcc -o test_gestore.exe src/main_test.o src/aula.o src/studente.o src/coda.o src/prenotazione.o

# Compilazione dei singoli file oggetto (.o) in C99
src/main.o: src/main.c
	gcc -c -std=c99 -I./include src/main.c -o src/main.o

src/main_test.o: src/main_test.c
	gcc -c -std=c99 -I./include src/main_test.c -o src/main_test.o

src/aula.o: src/aula.c
	gcc -c -std=c99 -I./include src/aula.c -o src/aula.o

src/studente.o: src/studente.c
	gcc -c -std=c99 -I./include src/studente.c -o src/studente.o

src/coda.o: src/coda.c
	gcc -c -std=c99 -I./include src/coda.c -o src/coda.o

src/prenotazione.o: src/prenotazione.c
	gcc -c -std=c99 -I./include src/prenotazione.c -o src/prenotazione.o
clean:
	rm -f src/*.o gestore_aula.exe test_gestore.exe
