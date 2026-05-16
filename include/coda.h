/**
 * File: coda.h
 * Autore: Domenico
 * Data: Aprile 2026
 *
 * Descrizione: Interfaccia dell'ADT Coda FIFO (First In, First Out).
 * Gestisce la lista d'attesa degli studenti quando l'aula è satura.
 */

#ifndef CODA_H // Se non è definita la macro di coda.h allora definisci coda.h.
#define CODA_H

#include "item.h"

/**
 * TIPO DI DATO ASTRATTO: Coda
 *
 * Implementato tramite puntatore a struct.
 */
typedef struct c_coda *Coda;

/**
 * Specifica sintattica:
 * Coda coda_crea(void);
 *
 * Specifica semantica:
 * Alloca dinamicamente e inizializza una nuova istanza di una coda vuota,
 * pronta a memorizzare elementi di tipo item.
 *
 * Pre-condizioni:
 * Nessuna.
 *
 * Post-condizioni:
 * Se l'allocazione ha successo, viene creata una coda vuota.
 *
 * Valore di ritorno:
 * Puntatore alla nuova Coda, oppure NULL se la memoria heap è satura.
 */
Coda coda_crea(void);

/**
 * Specifica sintattica:
 * int coda_vuota(Coda q);
 *
 * Specifica semantica:
 * Verifica se l'istanza della coda passata sia priva di elementi validi
 * al suo interno.
 *
 * Pre-condizioni:
 * La coda 'q' deve essere un puntatore valido (non NULL).
 *
 * Post-condizioni:
 * Lo stato della coda non subisce alcuna modifica.
 *
 * Valore di ritorno:
 * 1 se la coda è vuota, 0 se contiene elementi, -1 se il puntatore 'q' è NULL.
 */
int coda_vuota(Coda q);

/**
 * Specifica sintattica:
 * int coda_inserisci(item val, Coda q);
 *
 * Specifica semantica:
 * Accoda un nuovo elemento in fondo alla struttura rispettando la logica FIFO.
 *
 * Pre-condizioni:
 * La coda 'q' deve essere valida (non NULL) e 'val' deve essere un item
 * valido (diverso da NULLITEM).
 *
 * Post-condizioni:
 * La dimensione della coda incrementa di 1 unità. L'elemento 'val' diventa
 * l'ultimo elemento della coda.
 *
 * Valore di ritorno:
 * 1 in caso di successo, 0 se fallisce l'allocazione del nodo interno,
 * -1 se i parametri in ingresso non sono validi.
 */
int coda_inserisci(item val, Coda q);

/**
 * Specifica sintattica:
 * item coda_estrai(Coda q);
 *
 * Specifica semantica:
 * Rimuove l'elemento situato in testa alla coda e lo restituisce.
 *
 * Pre-condizioni:
 * La coda 'q' deve essere valida e non deve essere vuota.
 *
 * Post-condizioni:
 * La dimensione della coda decresce di 1 unità. Il secondo elemento
 * diventa la nuova testa della struttura.
 *
 * Valore di ritorno:
 * L'elemento (item) estratto dalla testa. Restituisce NULLITEM se la coda
 * è vuota o se si tenta di accedere a una coda non valida (NULL).
 */
item coda_estrai(Coda q);

/**
 * Specifica sintattica:
 * void coda_distruggi(Coda* q);
 *
 * Specifica semantica:
 * Svuota interamente la coda liberando tutti i nodi residui e dealloca
 * la struttura principale, azzerando il puntatore.
 *
 * Pre-condizioni:
 * L'indirizzo 'q' non deve essere NULL e '*q' deve puntare a una coda valida.
 *
 * Post-condizioni:
 * Tutta la memoria occupata viene restituita al sistema. Il puntatore
 * originale '*q' viene impostato a NULL per maggiore sicurezza.
 *
 * Valore di ritorno:
 * Nessuno (void).
 */
void coda_distruggi(Coda* q);

/**
 * Specifica sintattica:
 * void coda_stampa(Coda q);
 *
 * Specifica semantica:
 * Scorre la coda dalla testa al fondo stampando gli elementi tramite
 * la funzione di output specifica del tipo di dato memorizzato.
 *
 * Pre-condizioni:
 * La coda 'q' deve essere un puntatore valido (non NULL).
 *
 * Post-condizioni:
 * Lo stato interno della coda rimane inalterato.
 *
 * Valore di ritorno:
 * Nessuno (void).
 */
void coda_stampa(Coda q);

#endif
