/**
 * File: studente.c
 * Autore: Mathias
 * Data: Aprile 2026
 *
 * Descrizione: Implementazione dell'ADT Studente. Contiene la definizione
 * della struttura dati concreta e la logica delle funzioni di manipolazione.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "studente.h"

// Struttura dati interna. Le dimensioni degli array sono sovradimensionate
// per evitare troncamenti di nomi o corsi particolarmente lunghi.
struct studente {
    char matricola[20];
    char nome[100];
    char corso[100];
};

// Alloca e inizializza un nuovo studente copiando i parametri forniti.
Studente studente_crea(char* matricola, char* nome, char* corso) {
    // Si blocca l'esecuzione se i dati in ingresso sono mancanti,
    // evitando crash successivi nelle funzioni di copia stringa.
    if (matricola == NULL || nome == NULL || corso == NULL) {
        return NULL;
    }

    Studente s = (Studente) malloc(sizeof(struct studente));
    if (s == NULL) {
        return NULL;
    }

    // L'uso di strncpy previene vulnerabilità di buffer overflow.
    // Il terminatore '\0' è forzato manualmente perché strncpy non
    // lo garantisce se la stringa in input supera lo spazio disponibile.
    strncpy(s->matricola, matricola, sizeof(s->matricola) - 1);
    s->matricola[sizeof(s->matricola) - 1] = '\0';

    strncpy(s->nome, nome, sizeof(s->nome) - 1);
    s->nome[sizeof(s->nome) - 1] = '\0';

    strncpy(s->corso, corso, sizeof(s->corso) - 1);
    s->corso[sizeof(s->corso) - 1] = '\0';

    return s;
}

// Libera la memoria dinamica occupata dallo studente.
void studente_distruggi(Studente* s) {
    if (s != NULL && *s != NULL) {
        free(*s);
        *s = NULL;  // Annullamento puntatore per evitare memory dangling
    }
}

// Restituisce l'accesso in sola lettura alla matricola dello studente.
char* studente_ottieni_matricola(Studente s) {
    // Controllo per evitare dereferenziazioni di puntatori nulli.
    if (s == NULL) {
        return NULL;
    }
    return s->matricola;
}

// Stampa i dettagli anagrafici dello studente a video.
void studente_stampa(Studente s) {
    if (s != NULL) {
        printf("Matricola: %s, Nome: %s, Corso: %s\n", s->matricola, s->nome, s->corso);
    }
}

