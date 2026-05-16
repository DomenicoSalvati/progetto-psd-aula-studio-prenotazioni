/**
 * File: aula.c
 * Autori: Domenico e Mathias
 * Data: Maggio 2026
 *
 * Descrizione: Implementazione dell'ADT Aula. Integra un array per i posti,
 * una coda per l'attesa e una Tabella Hash. Utilizza liste concatenate (Chaining)
 * per la risoluzione delle collisioni al fine di mantenere l'accesso in O(1).
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "aula.h"
#include "coda.h"

#define DIMENSIONE_HASH 97

// Struttura che mappa l'elemento della tabella hash. Si usa il chaining 
// per assorbire in modo efficiente le continue cancellazioni degli studenti.
struct nodo_hash {
    char chiave[20];
    int indice_posto;
    struct nodo_hash *prossimo;
};

// Descrittore principale dell'aula (posti fisici + attesa + indice di ricerca rapida).
struct aula {
    char nome[100];
    int capienza_massima;
    int posti_occupati;
    Studente* posti;
    Coda lista_attesa;
    struct nodo_hash **tabella_ricerca;
};

// --- FUNZIONI PRIVATE PER INDICE HASH SECONDARIO ---

// Calcola l'indice della tabella (hashing) sfruttando l'algoritmo djb2.
static int calcola_hash(char *k, int m) {
    unsigned long hash = 5381;
    int c;
    while ((c = *k++)) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash % m;
}

// Inserisce in tabella una nuova mappatura tra matricola e numero di posto.
static int hash_inserisci(Aula a, char *chiave, int indice) {
    int idx;
    struct nodo_hash *head, *curr;

    idx = calcola_hash(chiave, DIMENSIONE_HASH);
    curr = head = a->tabella_ricerca[idx];

    // Controllo duplicati per mantenere la coerenza dell'indice
    while(curr) {
        if(strcmp(curr->chiave, chiave) == 0) return 0;
        curr = curr->prossimo;
    }

    struct nodo_hash *nuovo = malloc(sizeof(struct nodo_hash));
    if (nuovo == NULL) return 0; 

    strcpy(nuovo->chiave, chiave);
    nuovo->indice_posto = indice;

    // Inserimento in testa per mantenere l'operazione in tempo O(1)
    nuovo->prossimo = head;
    a->tabella_ricerca[idx] = nuovo;

    return 1;
}

// Cerca il nodo contenente il riferimento al posto, partendo dalla matricola.
static struct nodo_hash* hash_cerca(Aula a, char *chiave) {
    int idx = calcola_hash(chiave, DIMENSIONE_HASH);
    struct nodo_hash *curr = a->tabella_ricerca[idx];

    while(curr) {
        if(strcmp(curr->chiave, chiave) == 0) return curr;
        curr = curr->prossimo;
    }
    return NULL;
}

// Rimuove la matricola dalla tabella, ricollegando correttamente la catena.
static struct nodo_hash* hash_rimuovi(Aula a, char *chiave) {
    int idx;
    struct nodo_hash *prev, *curr, *head;

    idx = calcola_hash(chiave, DIMENSIONE_HASH);
    prev = curr = head = a->tabella_ricerca[idx];

    while (curr) {
        if(strcmp(curr->chiave, chiave) == 0) {
            // Estrazione sicura del nodo tramite il bypass dei puntatori
            if(curr == head) {
                a->tabella_ricerca[idx] = curr->prossimo;
            } else {
                prev->prossimo = curr->prossimo;
            }
            return curr;
        }
        prev = curr;
        curr = curr->prossimo;
    }
    return NULL;
}

// Libera iterativamente tutta la memoria occupata dai nodi di un singolo bucket.
static void distruggi_lista_hash(struct nodo_hash *p) {
    struct nodo_hash *corr = p;

    // L'approccio iterativo sostituisce la ricorsione per azzerare i rischi di stack overflow
    while (corr != NULL) {
        struct nodo_hash *temp = corr;
        corr = corr->prossimo;
        free(temp);
    }
}

// --- FINE FUNZIONI PRIVATE HASH ---

// Alloca e predispone tutte le strutture dati interne necessarie per la gestione dell'aula.
Aula aula_crea(char* nome, int capienza_massima) {
    if (nome == NULL || capienza_massima <= 0) return NULL;

    Aula a = (Aula) malloc(sizeof(struct aula));
    if (a == NULL) return NULL;

    strncpy(a->nome, nome, sizeof(a->nome) - 1);
    a->nome[sizeof(a->nome) - 1] = '\0';
    a->capienza_massima = capienza_massima;
    a->posti_occupati = 0;

    a->posti = (Studente*) calloc(capienza_massima, sizeof(Studente));
    a->lista_attesa = coda_crea();
    a->tabella_ricerca = calloc(DIMENSIONE_HASH, sizeof(struct nodo_hash *));

    // Rollback immediato se anche una sola delle strutture non ha memoria sufficiente
    if (a->posti == NULL || a->lista_attesa == NULL || a->tabella_ricerca == NULL) {
        if (a->posti) free(a->posti);
        if (a->lista_attesa) coda_distruggi(&(a->lista_attesa));
        if (a->tabella_ricerca) free(a->tabella_ricerca);
        free(a);
        return NULL;
    }

    return a;
}

// Dealloca gli studenti, svuota la coda, distrugge la tabella hash e l'aula stessa.
void aula_distruggi(Aula* a) {
    if (a != NULL && *a != NULL) {

        for (int i = 0; i < (*a)->capienza_massima; i++) {
            if ((*a)->posti[i] != NULL) {
                studente_distruggi(&((*a)->posti[i]));
            }
        }
        free((*a)->posti);

        coda_distruggi(&((*a)->lista_attesa));

        // Distruzione mirata di ogni bucket allocato per azzerare i memory leak
        for (int i = 0; i < DIMENSIONE_HASH; i++) {
            distruggi_lista_hash((*a)->tabella_ricerca[i]);
        }
        free((*a)->tabella_ricerca);

        free(*a);
        *a = NULL;
    }
}

// Assegna un posto allo studente se disponibile, oppure lo accoda in lista d'attesa.
int aula_ingresso(Aula a, Studente s) {
    if (a == NULL || s == NULL) return 0;

    if (a->posti_occupati < a->capienza_massima) {
        for (int i = 0; i < a->capienza_massima; i++) {
            if (a->posti[i] == NULL) {
                a->posti[i] = s;
                a->posti_occupati++;

                // L'inserimento simultaneo nell'hash permette ricerche future in O(1)
                hash_inserisci(a, studente_ottieni_matricola(s), i);
                return 1;
            }
        }
    }

    if (coda_inserisci(s, a->lista_attesa)) {
        return 2;
    }

    return 0;
}

// Ricerca lo studente tramite hash, libera il posto e fa subentrare chi è in coda.
int aula_uscita(Aula a, char* matricola_da_cercare) {
    if (a == NULL || matricola_da_cercare == NULL) return 0;

    // Ricerca diretta sull'indice secondario per evitare l'overhead dei cicli
    struct nodo_hash* trovato = hash_cerca(a, matricola_da_cercare);

    if (trovato != NULL) {
        int indice_posto = trovato->indice_posto;

        studente_distruggi(&(a->posti[indice_posto]));
        a->posti[indice_posto] = NULL;
        a->posti_occupati--;

        // Rimozione del nodo estratto dalla mappa logica
        struct nodo_hash* nodo_rimosso = hash_rimuovi(a, matricola_da_cercare);
        if (nodo_rimosso != NULL) free(nodo_rimosso);

        // Se c'è coda, si processa immediatamente il primo elemento in base alla logica FIFO
        if (!coda_vuota(a->lista_attesa)) {
            Studente prossimo = coda_estrai(a->lista_attesa);
            a->posti[indice_posto] = prossimo;
            a->posti_occupati++;

            hash_inserisci(a, studente_ottieni_matricola(prossimo), indice_posto);
        }
        return 1;
    }

    return 0;
}

// Produce una sintesi stampata dei posti fisici occupati e dello stato d'attesa.
void aula_stampa_stato(Aula a) {
    if (a != NULL) {
        printf("\n--- STATO AULA: %s ---\n", a->nome);
        printf("Posti occupati: %d/%d\n", a->posti_occupati, a->capienza_massima);

        for (int i = 0; i < a->capienza_massima; i++) {
            if (a->posti[i] != NULL) {
                printf("Posto %d: ", i + 1);
                studente_stampa(a->posti[i]);
            }
        }

        printf("\n--- LISTA D'ATTESA ---\n");
        if (coda_vuota(a->lista_attesa)) {
            printf("Nessuno in coda.\n");
        } else {
            coda_stampa(a->lista_attesa);
        }
    }
}
