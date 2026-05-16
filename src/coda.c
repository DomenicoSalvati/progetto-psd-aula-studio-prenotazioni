/**
 * File: coda.c
 * Autore: Domenico
 * Data: Aprile 2026
 *
 * Descrizione: Implementazione concreta dell'ADT Coda FIFO tramite
 * una struttura a lista semplicemente concatenata con puntatori alla testa e al fondo.
 */

#include <stdio.h>
#include <stdlib.h>
#include "item.h"
#include "coda.h"

// Struttura di supporto per il singolo anello della catena.
struct nodo {
    item valore;
    struct nodo *prossimo;
};

// Descrittore della coda che traccia gli estremi e il conteggio degli elementi.
struct c_coda {
    struct nodo *testa, *fondo;
    int n_elementi;
};

// Alloca e prepara per l'uso una nuova coda vuota.
Coda coda_crea(void) {
    struct c_coda *q = malloc(sizeof(struct c_coda));
    if (q == NULL) return NULL;

    q->n_elementi = 0;
    q->testa = NULL;
    q->fondo = NULL;
    return q;
}

// Verifica se la struttura d'attesa è priva di elementi.
int coda_vuota(Coda q) {
    if (q == NULL) return -1;
    return q->n_elementi == 0;
}

// Accoda un nuovo elemento in fondo alla struttura rispettando l'ordine FIFO.
int coda_inserisci(item val, Coda q) {
    if (q == NULL || val == NULLITEM) return -1;

    struct nodo *nuovo = malloc(sizeof(struct nodo));
    if (nuovo == NULL) return 0;

    nuovo->valore = val;
    nuovo->prossimo = NULL;

    // Aggancio differenziato in base allo stato (vuoto o popolato) della struttura
    if (q->testa == NULL) {
        q->testa = nuovo;
    } else {
        q->fondo->prossimo = nuovo;
    }

    q->fondo = nuovo; 
    (q->n_elementi)++;
    return 1;
}

// Rimuove e restituisce l'elemento in prima posizione nella coda.
item coda_estrai(Coda q) {
    // Blocco preventivo su strutture non allocate o già vuote
    if (q == NULL || q->n_elementi == 0) {
        return NULLITEM;
    }

    struct nodo *temp = q->testa;
    item risultato = temp->valore;

    // Avanzamento rapido in O(1)
    q->testa = q->testa->prossimo;

    // Se l'elemento estratto era l'unico presente, si resetta anche il fondo
    if (q->testa == NULL) {
        q->fondo = NULL;
    }

    free(temp); // Rilascio immediato del nodo per evitare memory leak
    (q->n_elementi)--;
    return risultato;
}

// Svuota sistematicamente la coda e dealloca il descrittore principale.
void coda_distruggi(Coda* q) {
    if (q != NULL && *q != NULL) {
        // La chiamata a coda_estrai gestisce autonomamente le free() dei nodi
        while (!coda_vuota(*q)) {
            coda_estrai(*q);
        }
        free(*q);
        *q = NULL;
    }
}

// Scorre la coda in ordine di uscita stampando i valori memorizzati.
void coda_stampa(Coda q) {
    if (q != NULL) {
        struct nodo *corr = q->testa;
        while (corr != NULL) {
            studente_stampa(corr->valore);
            corr = corr->prossimo;
        }
    }
}
