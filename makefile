all: gestore_aula test_gestore

# Creazione degli eseguibili (niente .exe su Linux/WSL)
gestore_aula: src/main.o src/aula.o src/studente.o src/coda.o src/prenotazione.o
	gcc -o gestore_aula src/main.o src/aula.o src/studente.o src/coda.o src/prenotazione.o

test_gestore: src/main_test.o src/aula.o src/studente.o src/coda.o src/prenotazione.o
	gcc -o test_gestore src/main_test.o src/aula.o src/studente.o src/coda.o src/prenotazione.o

# Compilazione dei singoli file oggetto (.o) con i .h come dipendenze
src/main.o: src/main.c include/aula.h include/studente.h
	gcc -c -std=c99 -Wall -pedantic -I./include src/main.c -o src/main.o

src/main_test.o: src/main_test.c include/aula.h include/studente.h
	gcc -c -std=c99 -Wall -pedantic -I./include src/main_test.c -o src/main_test.o

src/aula.o: src/aula.c include/aula.h include/studente.h include/coda.h include/prenotazione.h
	gcc -c -std=c99 -Wall -pedantic -I./include src/aula.c -o src/aula.o

src/studente.o: src/studente.c include/studente.h
	gcc -c -std=c99 -Wall -pedantic -I./include src/studente.c -o src/studente.o

src/coda.o: src/coda.c include/coda.h include/studente.h
	gcc -c -std=c99 -Wall -pedantic -I./include src/coda.c -o src/coda.o

src/prenotazione.o: src/prenotazione.c include/prenotazione.h include/studente.h
	gcc -c -std=c99 -Wall -pedantic -I./include src/prenotazione.c -o src/prenotazione.o

# Pulizia dei file compilati
clean:
	rm -f src/*.o gestore_aula test_gestore
