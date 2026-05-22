all: gestore_aula test_gestore

# Creazione degli eseguibili (niente .exe su Linux/WSL)
gestore_aula: object/main.o object/aula.o object/studente.o object/coda.o object/prenotazione.o
	gcc -o gestore_aula object/main.o object/aula.o object/studente.o object/coda.o object/prenotazione.o

test_gestore: object/main_test.o object/aula.o object/studente.o object/coda.o object/prenotazione.o
	gcc -o test_gestore object/main_test.o object/aula.o object/studente.o object/coda.o object/prenotazione.o

# Compilazione dei singoli file oggetto (.o) con i .h come dipendenze
object/main.o: src/main.c include/aula.h include/studente.h
	gcc -c -std=c99 -Wall -pedantic -I./include src/main.c -o object/main.o

object/main_test.o: src/main_test.c include/aula.h include/studente.h
	gcc -c -std=c99 -Wall -pedantic -I./include src/main_test.c -o object/main_test.o

object/aula.o: src/aula.c include/aula.h include/studente.h include/coda.h include/prenotazione.h
	gcc -c -std=c99 -Wall -pedantic -I./include src/aula.c -o object/aula.o
object/studente.o: src/studente.c include/studente.h
	gcc -c -std=c99 -Wall -pedantic -I./include src/studente.c -o object/studente.o

object/coda.o: src/coda.c include/coda.h include/studente.h
	gcc -c -std=c99 -Wall -pedantic -I./include src/coda.c -o object/coda.o

object/prenotazione.o: src/prenotazione.c include/prenotazione.h include/studente.h
	gcc -c -std=c99 -Wall -pedantic -I./include src/prenotazione.c -o object/prenotazione.o

# Pulizia dei file compilati
clean:
	rm -f object/*.o gestore_aula test_gestore
