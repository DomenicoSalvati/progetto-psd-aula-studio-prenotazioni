#ifndef CODA_H //controllo se è già definito coda.h
#define CODA_H

#include "item.h"

/* * Definisce il tipo Coda come puntatore alla struttura c_coda.
 * La struttura viene definita nel file .c per garantire information hiding.
 */
typedef struct c_coda *Coda;

/*
 * Specifica sintattica: Coda coda_crea(void);
 * Specifica semantica: Alloca e inizializza una nuova coda vuota.
 * Pre-condizioni: nessuna.
 * Post-condizioni: restituisce il puntatore alla coda, NULL se l'allocazione fallisce.
 */
Coda coda_crea(void);

/*
 * Specifica sintattica: int coda_vuota(Coda q);
 * Specifica semantica: Controlla se la coda è priva di elementi.
 * Pre-condizioni: q deve essere una coda valida (non NULL).
 * Post-condizioni: restituisce 1 se vuota, 0 se contiene elementi, -1 se q è NULL.
 */
int coda_vuota(Coda q);

/*
 * Specifica sintattica: item coda_estrai(Coda q);
 * Specifica semantica: Rimuove e restituisce l'elemento in testa alla coda.
 * Pre-condizioni: q non deve essere vuota.
 * Post-condizioni: restituisce l'elemento rimosso, NULLITEM se la coda è vuota.
 */
item coda_estrai(Coda q);

/*
 * Specifica sintattica: int coda_inserisci(item val, Coda q);
 * Specifica semantica: Inserisce un nuovo elemento in fondo alla coda.
 * Pre-condizioni: q deve essere valida, val non deve essere NULLITEM.
 * Post-condizioni: restituisce 1 se l'operazione riesce, 0 se l'allocazione fallisce, -1 se q è NULL.
 */
int coda_inserisci(item val, Coda q);

/*
 * Specifica sintattica: void coda_distruggi(Coda* q);
 * Specifica semantica: Libera tutta la memoria allocata per la coda e i suoi nodi.
 * Pre-condizioni: q != NULL, *q puntatore ad un'istanza valida.
 * Post-condizioni: *q viene deallocato e posto a NULL.
 */
void coda_distruggi(Coda* q);

#endif //se non è definito lo definisce
