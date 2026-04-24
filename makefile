all: gestore_aula.exe test_gestore.exe

# Creazione degli eseguibili finali
gestore_aula.exe: src/main.o src/aula.o src/studente.o
	gcc -o gestore_aula.exe src/main.o src/aula.o src/studente.o

test_gestore.exe: src/main_test.o src/aula.o src/studente.o
	gcc -o test_gestore.exe src/main_test.o src/aula.o src/studente.o

# Compilazione dei singoli file oggetto (.o) in C99
src/main.o: src/main.c
	gcc -c -std=c99 -I./include src/main.c -o src/main.o

src/main_test.o: src/main_test.c
	gcc -c -std=c99 -I./include src/main_test.c -o src/main_test.o

src/aula.o: src/aula.c
	gcc -c -std=c99 -I./include src/aula.c -o src/aula.o

src/studente.o: src/studente.c
	gcc -c -std=c99 -I./include src/studente.c -o src/studente.o

clean:
	rm -f src/*.o gestore_aula.exe test_gestore.exe
