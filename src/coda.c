/**
 * File: coda.c
 * Autore: Domenico
 * Data: creato Aprile 2026, fine implementazioni Maggio 2026.
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

/**
 * Alloca e prepara per l'uso una nuova coda vuota.
 *
 * Inizializza i puntatori di testa e fondo a NULL e azzera il contatore interno,
 * predisponendo la struttura per futuri inserimenti.
 *
 * Parametri:
 * Nessuno (void).
 *
 * Ritorna:
 * Il puntatore alla coda validamente creata, oppure NULL se la memoria heap è satura.
 */
Coda coda_crea(void) {
    struct c_coda *q = malloc(sizeof(struct c_coda));
    if (q == NULL) return NULL;

    q->n_elementi = 0;
    q->testa = NULL;
    q->fondo = NULL;
    return q;
}

/**
 * Verifica se la struttura d'attesa è attualmente priva di elementi.
 *
 * Utilizzata per prevenire estrazioni da code già vuote o per saltare stampe inutili.
 *
 * Parametri:
 * q: puntatore alla struttura coda da analizzare
 *
 * Ritorna:
 * 1 se la coda è vuota, 0 se contiene elementi, -1 se il puntatore non è valido.
 */
int coda_vuota(Coda q) {
    if (q == NULL) return -1;
    return q->n_elementi == 0;
}

/**
 * Accoda un nuovo elemento in fondo alla struttura.
 *
 * Garantisce l'integrità della logica FIFO (First In, First Out) aggiornando
 * correttamente sia la testa (se la coda era vuota) che il fondo della lista.
 *
 * Parametri:
 * val: l'elemento di tipo generico 'item' da memorizzare
 * q: puntatore alla coda di destinazione
 *
 * Ritorna:
 * 1 in caso di successo, 0 per errore di memoria, -1 per parametri non validi.
 */
int coda_inserisci(item val, Coda q) {
    if (q == NULL || val == NULLITEM) return -1;

    struct nodo *nuovo = malloc(sizeof(struct nodo));
    if (nuovo == NULL) return 0;

    nuovo->valore = val;
    nuovo->prossimo = NULL;

    if (q->testa == NULL) {
        q->testa = nuovo;
    } else {
        q->fondo->prossimo = nuovo;
    }

    q->fondo = nuovo;
    (q->n_elementi)++;
    return 1;
}

/**
 * Rimuove e restituisce l'elemento in prima posizione nella coda.
 *
 * Implementa l'estrazione veloce in O(1) modificando il puntatore di testa.
 * Libera in modo sicuro la memoria occupata dal nodo estratto per prevenire leak.
 *
 * Parametri:
 * q: puntatore alla coda da cui effettuare l'estrazione
 *
 * Ritorna:
 * L'oggetto 'item' rimosso, oppure NULLITEM se la coda è vuota o invalida.
 */
item coda_estrai(Coda q) {
    if (q == NULL || q->n_elementi == 0) {
        return NULLITEM;
    }

    struct nodo *temp = q->testa;
    item risultato = temp->valore;

    q->testa = q->testa->prossimo;

    if (q->testa == NULL) {
        q->fondo = NULL;
    }

    free(temp); 
    (q->n_elementi)--;
    return risultato;
}

/**
 * Svuota sistematicamente la coda e dealloca il descrittore principale.
 *
 * Esegue un'iterazione completa servendosi della funzione di estrazione per
 * smantellare progressivamente e in sicurezza tutta la memoria heap occupata.
 *
 * Parametri:
 * q: doppio puntatore alla coda da deallocare
 *
 * Ritorna:
 * Nessuno (void).
 */
void coda_distruggi(Coda* q) {
    if (q != NULL && *q != NULL) {
        while (!coda_vuota(*q)) {
            coda_estrai(*q);
        }
        free(*q);
        *q = NULL;
    }
}

/**
 * Scorre la coda in ordine di uscita stampando i valori memorizzati.
 *
 * Utile in fase di diagnostica per ottenere un'immagine dello stato d'attesa.
 * Delega la responsabilità della formattazione grafica alla funzione di stampa dell'item.
 *
 * Parametri:
 * q: puntatore alla coda da mostrare
 *
 * Ritorna:
 * Nessuno (void).
 */
void coda_stampa(Coda q) {
    if (q != NULL) {
        struct nodo *corr = q->testa;
        while (corr != NULL) {
            studente_stampa(corr->valore);
            corr = corr->prossimo;
        }
    }
}

/**
 * Interroga il descrittore per estrarre il conteggio aggiornato dei nodi in attesa.
 *
 * Mantiene il vincolo di complessità temporale O(1) poiché la variabile
 * n_elementi viene tenuta costantemente aggiornata durante inserimenti ed estrazioni.
 *
 * Parametri:
 * q: puntatore alla coda da ispezionare
 *
 * Ritorna:
 * L'intero che rappresenta gli elementi pendenti, oppure 0 in caso di coda nulla.
 */
int coda_lunghezza(Coda q) {
    if (q == NULL) return 0;
    return q->n_elementi;
}
