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

/**
 * Calcola l'indice della tabella (hashing) sfruttando l'algoritmo djb2.
 *
 * Utilizzata internamente per smistare gli studenti nei bucket dell'hash.
 *
 * Parametri:
 * chiave: la stringa da elaborare (matricola)
 * modulo: la dimensione della tabella hash
 *
 * Ritorna:
 * L'indice calcolato per l'inserimento.
 */
static int calcola_hash(char *chiave, int modulo) {
    unsigned long valore_hash = 5381;
    int carattere;
    while ((carattere = *chiave++)) {
        valore_hash = ((valore_hash << 5) + valore_hash) + carattere;
    }
    return valore_hash % modulo;
}

/**
 * Inserisce in tabella una nuova mappatura tra matricola e numero di posto.
 *
 * Viene invocata quando uno studente si siede fisicamente in un posto.
 *
 * Parametri:
 * a: puntatore all'oggetto Aula
 * chiave: matricola dello studente
 * indice_posto: il numero del posto assegnato
 *
 * Ritorna:
 * 1 in caso di successo, 0 in caso di fallimento o chiave già presente.
 */
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

/**
 * Cerca il nodo contenente il riferimento al posto, partendo dalla matricola.
 *
 * Garantisce un tempo di ricerca tendente a O(1) in media.
 *
 * Parametri:
 * a: puntatore all'oggetto Aula
 * chiave: matricola da cercare
 *
 * Ritorna:
 * Il puntatore al nodo hash individuato, oppure NULL se non presente.
 */
static struct nodo_hash* hash_cerca(Aula a, char *chiave) {
    int indice_hash = calcola_hash(chiave, DIMENSIONE_HASH);
    struct nodo_hash *corrente = a->tabella_ricerca[indice_hash];

    while(corrente) {
        if(strcmp(corrente->chiave, chiave) == 0) return corrente;
        corrente = corrente->prossimo;
    }
    return NULL;
}

/**
 * Rimuove la matricola dalla tabella, ricollegando correttamente la catena.
 *
 * Fondamentale durante l'uscita dello studente per svuotare il registro hash.
 *
 * Parametri:
 * a: puntatore all'oggetto Aula
 * chiave: matricola da rimuovere
 *
 * Ritorna:
 * Il puntatore al nodo estratto (per la successiva deallocazione).
 */
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

/**
 * Libera iterativamente tutta la memoria occupata dai nodi di un singolo bucket.
 *
 * Funzione di supporto chiamata durante la fase di distruzione dell'aula.
 *
 * Parametri:
 * p: puntatore al primo nodo della lista concatenata
 *
 * Ritorna:
 * Nessuno (void).
 */
static void distruggi_lista_hash(struct nodo_hash *p) {
    struct nodo_hash *corrente = p;
    while (corrente != NULL) {
        struct nodo_hash *temporaneo = corrente;
        corrente = corrente->prossimo;
        free(temporaneo);
    }
}

// --- FINE FUNZIONI PRIVATE HASH ---

/**
 * Calcola se due fasce orarie si accavallano temporalmente.
 *
 * Analizza le stringhe delle fasce orarie e converte i tempi in minuti
 * per eseguire un confronto matematico e prevenire doppie prenotazioni.
 *
 * Parametri:
 * f1: puntatore alla stringa della prima fascia oraria
 * f2: puntatore alla stringa della seconda fascia oraria
 *
 * Ritorna:
 * 1 se le fasce si sovrappongono, 0 altrimenti.
 */
static int controllo_sovrapposizione(const char* f1, const char* f2) {
    int in1_h, in1_m, out1_h, out1_m;
    int in2_h, in2_m, out2_h, out2_m;

    sscanf(f1, "%d:%d-%d:%d", &in1_h, &in1_m, &out1_h, &out1_m);
    sscanf(f2, "%d:%d-%d:%d", &in2_h, &in2_m, &out2_h, &out2_m);

    int inizio1 = in1_h * 60 + in1_m;
    int fine1 = out1_h * 60 + out1_m;
    int inizio2 = in2_h * 60 + in2_m;
    int fine2 = out2_h * 60 + out2_m;

    // Due intervalli si sovrappongono se il primo inizia prima che il secondo finisca,
    // e il secondo inizia prima che il primo finisca.
    if (inizio1 < fine2 && inizio2 < fine1) {
        return 1;
    }
    return 0;
}

/**
 * Alloca e predispone tutte le strutture dati interne necessarie.
 *
 * Inizializza a vuoto l'array dei posti fisici, la coda e la tabella hash,
 * implementando un sistema di fallback per la pulizia della memoria in caso di errori.
 *
 * Parametri:
 * nome: stringa identificativa dell'aula
 * capienza_massima: numero di posti totali
 *
 * Ritorna:
 * Il puntatore all'istanza creata, oppure NULL in caso di fallimento dell'allocazione.
 */
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

/**
 * Dealloca gli studenti, svuota la coda, distrugge la tabella hash e il registro.
 *
 * Scorre le prenotazioni e distrugge gli studenti che non hanno effettuato l'accesso,
 * azzerando infine il puntatore principale.
 *
 * Parametri:
 * a: doppio puntatore all'oggetto Aula
 *
 * Ritorna:
 * Nessuno (void).
 */
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

/**
 * Assegna un posto specifico allo studente se disponibile, oppure lo accoda.
 *
 * Verifica rigorosamente che l'indice richiesto sia entro i limiti dell'aula.
 * Se il posto fisico e' vuoto lo occupa, altrimenti smista lo studente nella lista d'attesa.
 *
 * Parametri:
 * a: puntatore all'oggetto Aula
 * s: puntatore allo Studente
 * posto_richiesto: l'intero che identifica la sedia scelta
 *
 * Ritorna:
 * 1 (ingresso effettuato), 2 (messo in coda), 0 (errore o posto non valido).
 */
int aula_ingresso(Aula a, Studente s, int posto_richiesto) {
    if (a == NULL || s == NULL) return 0;

    if (posto_richiesto < 0 || posto_richiesto >= a->capienza_massima) return 0;

    if (a->posti_occupati < a->capienza_massima && a->posti[posto_richiesto] == NULL) {
        a->posti[posto_richiesto] = s;
        a->posti_occupati++;

        hash_inserisci(a, studente_ottieni_matricola(s), posto_richiesto);
        return 1;
    }

    if (coda_inserisci(s, a->lista_attesa)) {
        return 2;
    }

    return 0;
}

/**
 * Ricerca lo studente tramite hash, libera il posto e fa subentrare chi è in coda.
 *
 * Intercetta il nodo rimosso dalla tabella e lo dealloca in sicurezza, gestendo
 * l'ingresso automatico del primo studente pendente nel posto appena liberato.
 *
 * Parametri:
 * a: puntatore all'oggetto Aula
 * matricola_da_cercare: la matricola dell'utente che lascia l'aula
 *
 * Ritorna:
 * 1 in caso di successo, 0 se la matricola non viene trovata.
 */
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

/**
 * Produce una sintesi stampata dei posti fisici occupati e dello stato d'attesa.
 *
 * Itera sull'array dei posti mostrando esclusivamente quelli in cui siede qualcuno,
 * per poi generare a schermo anche l'attuale situazione della lista d'attesa virtuale.
 *
 * Parametri:
 * a: puntatore all'oggetto Aula
 *
 * Ritorna:
 * Nessuno (void).
 */
void aula_stampa_stato(Aula a) {
    if (a != NULL) {
        printf("\n--- STATO AULA: %s ---\n", a->nome);
        printf("Posti occupati: %d/%d\n", a->posti_occupati, a->capienza_massima);

        for (int i = 0; i < a->capienza_massima; i++) {
            if (a->posti[i] != NULL) {
                printf("Posto %d: ", i);
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

/**
 * Registra la pratica di prenotazione prevenendo i conflitti di orario.
 *
 * Controlla che il posto richiesto non sia già stato prenotato da qualcun altro
 * per la medesima data e in una fascia oraria sovrapposta, iterando sullo storico.
 *
 * Parametri:
 * a: puntatore all'oggetto Aula
 * p: oggetto Prenotazione da registrare
 *
 * Ritorna:
 * 1 se allocato con successo, 0 in caso di conflitti o errore.
 */
int aula_aggiungi_prenotazione(Aula a, Prenotazione p) {
    if (a == NULL || p == NULL) return 0;

    struct nodo_prenotazione* corrente = a->registro_prenotazioni;
    while(corrente != NULL) {
        // Se stesso posto, stessa data e la prenotazione non è annullata...
        if (prenotazione_ottieni_posto(corrente->pratica) == prenotazione_ottieni_posto(p) &&
            strcmp(prenotazione_ottieni_data(corrente->pratica), prenotazione_ottieni_data(p)) == 0 &&
            prenotazione_ottieni_stato(corrente->pratica) != -1) {

            // ...controlliamo matematicamente se le ore si scontrano
            if (controllo_sovrapposizione(prenotazione_ottieni_fascia(corrente->pratica), prenotazione_ottieni_fascia(p))) {
                return 0; // Conflitto di orario/sedia intercettato!
            }
        }
        corrente = corrente->prossimo;
    }

    struct nodo_prenotazione* nuovo = malloc(sizeof(struct nodo_prenotazione));
    if (nuovo == NULL) return 0;

    nuovo->pratica = p;
    nuovo->prossimo = a->registro_prenotazioni;
    a->registro_prenotazioni = nuovo;

    return 1;
}

/**
 * Convalida l'arrivo ricercando la pratica e inoltrando lo studente al sistema fisico.
 *
 * Impedisce doppie convalide verificando lo stato di check-in della pratica e
 * indirizza lo studente verso il posto specifico registrato precedentemente.
 *
 * Parametri:
 * a: puntatore all'oggetto Aula
 * matricola: stringa che identifica la prenotazione da convalidare
 *
 * Ritorna:
 * 1 (check-in e seduto), 2 (check-in e in coda), -1 (check-in già fatto), 0 (errore).
 */
int aula_checkin_prenotazione(Aula a, char* matricola) {
    if (a == NULL || matricola == NULL) return 0;

    struct nodo_prenotazione* corrente = a->registro_prenotazioni;

    while (corrente != NULL) {
        Studente s = prenotazione_ottieni_studente(corrente->pratica);
        if (strcmp(studente_ottieni_matricola(s), matricola) == 0) {

            if (prenotazione_ottieni_stato(corrente->pratica) == 0) {
                prenotazione_effettua_checkin(corrente->pratica);

                int posto_riservato = prenotazione_ottieni_posto(corrente->pratica);

                return aula_ingresso(a, s, posto_riservato);
            } else {
                return -1;
            }
        }
        corrente = corrente->prossimo;
    }
    return 0;
}

/**
 * Elimina una prenotazione pendente prevenendo l'occupazione inutile della memoria.
 *
 * Ricerca la pratica nel registro, garantendo l'integrità dei puntatori circostanti e
 * bloccando l'annullamento qualora lo studente sia già subentrato fisicamente in aula.
 *
 * Parametri:
 * a: puntatore all'oggetto Aula
 * matricola: stringa che identifica la prenotazione da annullare
 *
 * Ritorna:
 * 1 in caso di successo, 0 se la pratica non esiste o non è modificabile.
 */
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

/**
 * Genera un report analitico dettagliato per il monitoraggio dell'aula.
 *
 * Include il conteggio degli studenti in attesa e una sintesi delle presenze
 * raggruppate per singola fascia oraria.
 *
 * Parametri:
 * a: puntatore all'oggetto Aula
 *
 * Ritorna:
 * Nessuno (void).
 */
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

    printf("Studenti attualmente in coda d'attesa: %d\n", coda_lunghezza(a->lista_attesa));

    printf("\n--- OCCUPAZIONE PER FASCIA ORARIA ---\n");
    corrente = a->registro_prenotazioni;
    while(corrente != NULL) {
        if(prenotazione_ottieni_stato(corrente->pratica) == 1) {
            printf("- Fascia %s | Data: %s | Posto occupato: %02d\n",
                   prenotazione_ottieni_fascia(corrente->pratica),
                   prenotazione_ottieni_data(corrente->pratica),
                   prenotazione_ottieni_posto(corrente->pratica));
        }
        corrente = corrente->prossimo;
    }
    printf("===========================================\n");
}

/**
 * Verifica l'effettiva presenza di uno studente all'interno della struttura.
 *
 * Utilizza la tabella hash interna per restituire una risposta immediata,
 * permettendo di filtrare i duplicati.
 *
 * Parametri:
 * a: puntatore all'oggetto Aula
 * matricola: stringa che identifica lo studente da ricercare
 *
 * Ritorna:
 * 1 qualora la matricola esista già, 0 altrimenti.
 */
int aula_studente_esiste(Aula a, char* matricola){
	if(a == NULL || matricola == NULL){
        return 0;
    }

	if(hash_cerca(a, matricola) != NULL){
        return 1;
    }
	return 0;
}
