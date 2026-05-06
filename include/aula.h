#ifndef AULA_H //controllo se è già definito aula.h
#define AULA_H

#include "studente.h"

typedef struct aula* Aula;

/*
 * Specifica sintattica: Aula aula_crea(char* nome, int capienza_massima);
 * Specifica semantica: Alloca e inizializza una nuova istanza di Aula.
 * Pre-condizioni: nome != NULL, capienza_massima > 0.
 * Post-condizioni: Restituisce l'istanza allocata, o NULL in caso di errore.
 */
Aula aula_crea(char* nome, int capienza_massima);

/*
 * Specifica sintattica: void aula_distruggi(Aula* a);
 * Specifica semantica: Dealloca un'istanza di Aula e la memoria ad essa associata.
 * Pre-condizioni: a != NULL, *a puntatore ad un'istanza valida.
 * Post-condizioni: *a viene deallocato e posto a NULL.
 */
void aula_distruggi(Aula* a);

/*
 * Specifica sintattica: int aula_ingresso_libero(Aula a, Studente s);
 * Specifica semantica: Inserisce uno studente nel primo posto disponibile dell'aula.
 * Pre-condizioni: a != NULL, s != NULL.
 * Post-condizioni: Restituisce 1 se inserito, 0 se l'aula è piena o in caso di errore.
 */
int aula_ingresso_libero(Aula a, Studente s);

/*
 * Specifica sintattica: int aula_uscita(Aula a, Studente s);
 * Specifica semantica: Rimuove uno studente dall'aula liberando il suo posto.
 * Pre-condizioni: a != NULL, s != NULL.
 * Post-condizioni: Restituisce 1 se rimosso, 0 se lo studente non era presente o in caso di errore.
 */
int aula_uscita(Aula a, Studente s);

#endif //se non è già definito allora lo definisce
