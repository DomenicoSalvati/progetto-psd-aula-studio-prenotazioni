/**
 * File: aula.h
 * Autori: Domenico e Mathias
 * Data: Maggio 2026
 *
 * Descrizione: Interfaccia per l'ADT Aula. Definisce il tipo opaco e le
 * operazioni per gestire i posti a sedere, la coda d'attesa degli studenti
 * e la ricerca rapida tramite la Tabella Hash (Chaining).
 */

#ifndef AULA_H //Se non è definita la macro di aula.h allora definisci aula.h
#define AULA_H

#include "studente.h"

/**
 * TIPO DI DATO ASTRATTO: Aula
 *
 * Implementato tramite puntatore opaco a struct per garantire l'information
 * hiding. Nasconde la gestione degli array, della coda d'attesa e della
 * tabella hash interna.
 */
typedef struct aula* Aula;


/**
 * Specifica sintattica:
 * Aula aula_crea(char* nome, int capienza_massima);
 *
 * Specifica semantica:
 * Alloca dinamicamente la memoria per un nuovo oggetto Aula, configurando
 * l'array dei posti, la coda d'attesa e la tabella hash di ricerca.
 *
 * Pre-condizioni:
 * Il parametro 'nome' deve essere una stringa valida (non NULL).
 * Il parametro 'capienza_massima' deve essere un intero maggiore di zero.
 *
 * Post-condizioni:
 * Viene creato un nuovo oggetto Aula inizializzato e pronto all'uso.
 *
 * Valore di ritorno:
 * Puntatore alla nuova Aula creata. Restituisce NULL se uno dei parametri
 * non è valido o se fallisce l'allocazione della memoria nell'heap.
 */
Aula aula_crea(char* nome, int capienza_massima);


/**
 * Specifica sintattica:
 * void aula_distruggi(Aula* a);
 *
 * Specifica semantica:
 * Rilascia l'intera memoria occupata dall'aula, distruggendo tutti gli oggetti
 * studente seduti, svuotando la coda e deallocando tutti i nodi della tabella hash.
 *
 * Pre-condizioni:
 * L'indirizzo 'a' non deve essere NULL e '*a' deve puntare a un oggetto
 * Aula valido allocato in precedenza.
 *
 * Post-condizioni:
 * Tutta la memoria associata all'aula viene liberata. Il puntatore '*a'
 * viene impostato a NULL per prevenire dangling pointers.
 *
 * Valore di ritorno:
 * Nessuno (void).
 */
void aula_distruggi(Aula* a);


/**
 * Specifica sintattica:
 * int aula_ingresso(Aula a, Studente s);
 *
 * Specifica semantica:
 * Gestisce l'ingresso di uno studente. Se c'è un posto fisico libero, lo studente
 * viene fatto sedere e mappato nella tabella hash. Se l'aula è satura, viene
 * inserito in coda d'attesa rispettando la logica FIFO.
 *
 * Pre-condizioni:
 * L'oggetto 'a' e l'oggetto 's' devono essere puntatori validi (non NULL).
 *
 * Post-condizioni:
 * Lo studente viene inserito nei posti a sedere o nella coda d'attesa.
 *
 * Valore di ritorno:
 * Restituisce 1 se lo studente si è seduto, 2 se è stato inserito in coda
 * d'attesa, 0 in caso di errore o parametri non validi.
 */
int aula_ingresso(Aula a, Studente s);


/**
 * Specifica sintattica:
 * int aula_uscita(Aula a, char* matricola_da_cercare);
 *
 * Specifica semantica:
 * Rimuove uno studente dall'aula identificandolo tramite la matricola. La ricerca
 * avviene in tempo costante O(1) grazie alla tabella hash. Se rimosso, viene
 * fatto subentrare automaticamente il primo studente in coda d'attesa.
 *
 * Pre-condizioni:
 * L'oggetto 'a' e la stringa 'matricola_da_cercare' non devono essere NULL.
 *
 * Post-condizioni:
 * Lo studente esce dall'aula. Il suo posto viene liberato e, se presente qualcuno
 * in coda, viene occupato dal nuovo subentrante (aggiornando la tabella hash).
 *
 * Valore di ritorno:
 * Restituisce 1 se l'operazione di uscita ha successo, 0 se lo studente
 * con quella specifica matricola non era presente nell'aula.
 */
int aula_uscita(Aula a, char* matricola_da_cercare);


/**
 * Specifica sintattica:
 * void aula_stampa_stato(Aula a);
 *
 * Specifica semantica:
 * Mostra sullo standard output lo stato corrente dell'aula, elencando tutti
 * i posti occupati con i dettagli degli studenti e lo stato della coda d'attesa.
 *
 * Pre-condizioni:
 * L'oggetto 'a' deve essere un puntatore valido (non NULL).
 *
 * Post-condizioni:
 * Lo stato interno dell'aula e della tabella hash rimane del tutto inalterato.
 *
 * Valore di ritorno:
 * Nessuno (void).
 */
void aula_stampa_stato(Aula a);

#endif
