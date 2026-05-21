/**
 * File: aula.c
 * Autori: Domenico e Mathias
 * Data: creato Aprile 2026, fine implementazioni Maggio 2026
 *
 * Descrizione: Implementazione dell'ADT Aula. Integra un array per i posti,
 * una coda per l'attesa e una Tabella Hash. Utilizza liste concatenate (Chaining)
 * per la risoluzione delle collisioni al fine di mantenere l'accesso in O(1).
 * Gestisce inoltre il registro storico delle prenotazioni.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "aula.h"
#include "coda.h"

#define DIMENSIONE_HASH 97

// Struttura che mappa l'elemento della tabella hash.
struct nodo_hash {
    char chiave[20];
    int indice_posto;
    struct nodo_hash *prossimo;
};

// Struttura per conservare il registro burocratico delle prenotazioni.
struct nodo_prenotazione {
    Prenotazione pratica;
    struct nodo_prenotazione *prossimo;
};

// Descrittore principale dell'aula (posti fisici + attesa + indice di ricerca + registro).
struct aula {
    char nome[100];
    int capienza_massima;
    int posti_occupati;
    Studente* posti;
    Coda lista_attesa;
    struct nodo_hash **tabella_ricerca;
    struct nodo_prenotazione *registro_prenotazioni;
};

// --- FUNZIONI PRIVATE PER INDICE HASH SECONDARIO ---

// Calcola l'indice della tabella (hashing) sfruttando l'algoritmo djb2.
static int calcola_hash(char *chiave, int modulo) {
    unsigned long valore_hash = 5381;
    int carattere;
    while ((carattere = *chiave++)) {
        valore_hash = ((valore_hash << 5) + valore_hash) + carattere;
    }
    return valore_hash % modulo;
}

// Inserisce in tabella una nuova mappatura tra matricola e numero di posto.
static int hash_inserisci(Aula a, char *chiave, int indice_posto) {
    int indice_hash;
    struct nodo_hash *testa, *corrente;

    indice_hash = calcola_hash(chiave, DIMENSIONE_HASH);
    corrente = testa = a->tabella_ricerca[indice_hash];

    while(corrente) {
        if(strcmp(corrente->chiave, chiave) == 0) return 0;
        corrente = corrente->prossimo;
    }

    struct nodo_hash *nuovo = malloc(sizeof(struct nodo_hash));
    if (nuovo == NULL) return 0;

    strcpy(nuovo->chiave, chiave);
    nuovo->indice_posto = indice_posto;

    nuovo->prossimo = testa;
    a->tabella_ricerca[indice_hash] = nuovo;

    return 1;
}

// Cerca il nodo contenente il riferimento al posto, partendo dalla matricola.
static struct nodo_hash* hash_cerca(Aula a, char *chiave) {
    int indice_hash = calcola_hash(chiave, DIMENSIONE_HASH);
    struct nodo_hash *corrente = a->tabella_ricerca[indice_hash];

    while(corrente) {
        if(strcmp(corrente->chiave, chiave) == 0) return corrente;
        corrente = corrente->prossimo;
    }
    return NULL;
}

// Rimuove la matricola dalla tabella, ricollegando correttamente la catena.
static struct nodo_hash* hash_rimuovi(Aula a, char *chiave) {
    int indice_hash;
    struct nodo_hash *precedente, *corrente, *testa;

    indice_hash = calcola_hash(chiave, DIMENSIONE_HASH);
    precedente = corrente = testa = a->tabella_ricerca[indice_hash];

    while (corrente) {
        if(strcmp(corrente->chiave, chiave) == 0) {
            if(corrente == testa) {
                a->tabella_ricerca[indice_hash] = corrente->prossimo;
            } else {
                precedente->prossimo = corrente->prossimo;
            }
            return corrente;
        }
        precedente = corrente;
        corrente = corrente->prossimo;
    }
    return NULL;
}

// Libera iterativamente tutta la memoria occupata dai nodi di un singolo bucket.
static void distruggi_lista_hash(struct nodo_hash *p) {
    struct nodo_hash *corrente = p;
    while (corrente != NULL) {
        struct nodo_hash *temporaneo = corrente;
        corrente = corrente->prossimo;
        free(temporaneo);
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
    a->registro_prenotazioni = NULL;

    if (a->posti == NULL || a->lista_attesa == NULL || a->tabella_ricerca == NULL) {
        if (a->posti) free(a->posti);
        if (a->lista_attesa) coda_distruggi(&(a->lista_attesa));
        if (a->tabella_ricerca) free(a->tabella_ricerca);
        free(a);
        return NULL;
    }

    return a;
}

// Dealloca gli studenti, svuota la coda, distrugge la tabella hash e il registro.
void aula_distruggi(Aula* a) {
    if (a != NULL && *a != NULL) {

        for (int i = 0; i < (*a)->capienza_massima; i++) {
            if ((*a)->posti[i] != NULL) {
                studente_distruggi(&((*a)->posti[i]));
            }
        }
        free((*a)->posti);

        coda_distruggi(&((*a)->lista_attesa));

        for (int i = 0; i < DIMENSIONE_HASH; i++) {
            distruggi_lista_hash((*a)->tabella_ricerca[i]);
        }
        free((*a)->tabella_ricerca);

        struct nodo_prenotazione* prenotazione_corrente = (*a)->registro_prenotazioni;
        while(prenotazione_corrente != NULL) {
            struct nodo_prenotazione* temporaneo = prenotazione_corrente;
            prenotazione_corrente = prenotazione_corrente->prossimo;

            if (prenotazione_ottieni_stato(temporaneo->pratica) == 0) {
                Studente studente_pendente = prenotazione_ottieni_studente(temporaneo->pratica);
                studente_distruggi(&studente_pendente);
            }
            prenotazione_distruggi(&(temporaneo->pratica));
            free(temporaneo);
        }

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

    struct nodo_hash* trovato = hash_cerca(a, matricola_da_cercare);

    if (trovato != NULL) {
        int indice_posto = trovato->indice_posto;

        studente_distruggi(&(a->posti[indice_posto]));
        a->posti[indice_posto] = NULL;
        a->posti_occupati--;

        struct nodo_hash* nodo_rimosso = hash_rimuovi(a, matricola_da_cercare);
        if (nodo_rimosso != NULL) free(nodo_rimosso);

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

// Registra la pratica di prenotazione inserendola in testa al registro storico.
int aula_aggiungi_prenotazione(Aula a, Prenotazione p) {
    if (a == NULL || p == NULL) return 0;

    struct nodo_prenotazione* nuovo = malloc(sizeof(struct nodo_prenotazione));
    if (nuovo == NULL) return 0;

    nuovo->pratica = p;
    nuovo->prossimo = a->registro_prenotazioni;
    a->registro_prenotazioni = nuovo;

    return 1;
}

// Convalida l'arrivo ricercando la pratica e inoltrando lo studente al sistema fisico.
int aula_checkin_prenotazione(Aula a, char* matricola) {
    if (a == NULL || matricola == NULL) return 0;

    struct nodo_prenotazione* corrente = a->registro_prenotazioni;

    while (corrente != NULL) {
        Studente s = prenotazione_ottieni_studente(corrente->pratica);
        if (strcmp(studente_ottieni_matricola(s), matricola) == 0) {

            if (prenotazione_ottieni_stato(corrente->pratica) == 0) {
                prenotazione_effettua_checkin(corrente->pratica);
                return aula_ingresso(a, s);
            } else {
                return -1; 
            }
        }
        corrente = corrente->prossimo;
    }
    return 0;
}

// Elimina una prenotazione pendente prevenendo l'occupazione inutile della memoria.
int aula_annulla_prenotazione(Aula a, char* matricola) {
    if (a == NULL || matricola == NULL) return 0;

    struct nodo_prenotazione* corrente = a->registro_prenotazioni;
    struct nodo_prenotazione* precedente = NULL;

    while(corrente != NULL) {
        Studente s = prenotazione_ottieni_studente(corrente->pratica);

        if (strcmp(studente_ottieni_matricola(s), matricola) == 0) {
            if (prenotazione_ottieni_stato(corrente->pratica) == 1) return 0;

            if (precedente == NULL) {
                a->registro_prenotazioni = corrente->prossimo;
            } else {
                precedente->prossimo = corrente->prossimo;
            }

            studente_distruggi(&s);
            prenotazione_distruggi(&(corrente->pratica));
            free(corrente);
            return 1;
        }
        precedente = corrente;
        corrente = corrente->prossimo;
    }
    return 0;
}

// Genera un report analitico iterando sul registro storico.
void aula_stampa_report_prenotazioni(Aula a) {
    if (a == NULL) return;

    int totali = 0;
    int accessi_effettivi = 0;
    int assenti = 0;

    struct nodo_prenotazione* corrente = a->registro_prenotazioni;

    printf("\n=== REPORT STATISTICO E STORICO ACCESSI ===\n");
    while(corrente != NULL) {
        totali++;
        if (prenotazione_ottieni_stato(corrente->pratica) == 1) {
            accessi_effettivi++;
        } else {
            assenti++;
        }
        prenotazione_stampa(corrente->pratica);
        corrente = corrente->prossimo;
    }

    printf("\n--- SINTESI DATI ---\n");
    printf("Totale Prenotazioni processate: %d\n", totali);
    printf("Accessi Effettivi (Check-in completato): %d\n", accessi_effettivi);
    printf("Assenti / In attesa di arrivo: %d\n", assenti);
    printf("===========================================\n");
}




int aula_studente_esiste(Aula a, char* matricola){
	if(a == NULL || matricola == NULL){
				 return 0;
				}

	if(hash_cerca(a, matricola) != NULL){
				 return 1; // La matricola esiste già
				}
	return 0;
}
