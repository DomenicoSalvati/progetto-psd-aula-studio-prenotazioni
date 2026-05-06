#ifndef AULA_H //controlla se è stato definito già il file tramite una macro
#define AULA_H

#include "studente.h"

typedef struct aula* Aula;

/*
 * Specifica sintattica: Aula aula_crea(char* nome, int capienza_massima);
 * Specifica semantica: Alloca e inizializza una nuova istanza di Aula, inclusa la lista d'attesa.
 * Pre-condizioni: nome != NULL, capienza_massima > 0.
 * Post-condizioni: Restituisce l'istanza allocata, o NULL in caso di errore di allocazione.
 */
Aula aula_crea(char* nome, int capienza_massima);

/*
 * Specifica sintattica: void aula_distruggi(Aula* a);
 * Specifica semantica: Dealloca un'istanza di Aula, i suoi posti e svuota la lista d'attesa.
 * Pre-condizioni: a != NULL, *a puntatore ad un'istanza valida.
 * Post-condizioni: *a viene deallocato completamente e posto a NULL.
 */
void aula_distruggi(Aula* a);

/*
 * Specifica sintattica: int aula_ingresso(Aula a, Studente s);
 * Specifica semantica: Inserisce uno studente nel primo posto disponibile, o in coda se l'aula è piena.
 * Pre-condizioni: a != NULL, s != NULL.
 * Post-condizioni: Restituisce 1 se seduto nei posti, 2 se inserito in coda d'attesa, 0 in caso di errore.
 */
int aula_ingresso(Aula a, Studente s);

/*
 * Specifica sintattica: int aula_uscita(Aula a, Studente s);
 * Specifica semantica: Rimuove uno studente dall'aula liberando il posto e fa subentrare il primo in attesa, se presente.
 * Pre-condizioni: a != NULL, s != NULL.
 * Post-condizioni: Restituisce 1 se rimosso con successo, 0 se lo studente non era presente o errore.
 */
int aula_uscita(Aula a, Studente s);

/*
 * Specifica sintattica: void aula_stampa_stato(Aula a);
 * Specifica semantica: Stampa l'elenco degli studenti presenti e lo stato della lista d'attesa.
 * Pre-condizioni: a != NULL.
 * Post-condizioni: Lo stato dell'aula viene stampato sullo standard output.
 */
void aula_stampa_stato(Aula a);

#endif //se non è stato definito allora lo definisce
