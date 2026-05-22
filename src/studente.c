/**
 * File: studente.c
 * Autore: Mathias
 * Data: creato Aprile 2026, fine implementazione Maggio 2026.
 *
 * Descrizione: Implementazione dell'ADT Studente. Contiene la definizione
 * della struttura dati concreta e la logica delle funzioni di manipolazione.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "studente.h"

// Struttura dati nascosta per mantenere incapsulati i dettagli anagrafici.
// Le dimensioni sono sovradimensionate per evitare troncamenti di stringhe lunghe.
struct studente {
    char matricola[20];
    char nome[100];
    char corso[100];
};

/**
 * Alloca e inizializza un nuovo studente copiando i parametri forniti.
 *
 * Blocca l'esecuzione se i dati in ingresso sono mancanti, evitando crash successivi,
 * e utilizza la funzione strncpy per prevenire vulnerabilità di buffer overflow.
 *
 * Parametri:
 * matricola: stringa alfanumerica identificativa
 * nome: stringa contenente il nome dello studente
 * corso: stringa contenente il corso di laurea
 *
 * Ritorna:
 * Il puntatore all'istanza di Studente creata, oppure NULL in caso di errore o dati mancanti.
 */
Studente studente_crea(char* matricola, char* nome, char* corso) {
    if (matricola == NULL || nome == NULL || corso == NULL) {
        return NULL;
    }

    Studente s = (Studente) malloc(sizeof(struct studente));
    if (s == NULL) {
        return NULL;
    }

    strncpy(s->matricola, matricola, sizeof(s->matricola) - 1);
    s->matricola[sizeof(s->matricola) - 1] = '\0';

    strncpy(s->nome, nome, sizeof(s->nome) - 1);
    s->nome[sizeof(s->nome) - 1] = '\0';

    strncpy(s->corso, corso, sizeof(s->corso) - 1);
    s->corso[sizeof(s->corso) - 1] = '\0';

    return s;
}

/**
 * Libera la memoria dinamica occupata dal descrittore dello studente.
 *
 * Smantella l'oggetto e azzera il puntatore di origine per prevenire il
 * fenomeno dei memory leak e dei dangling pointer.
 *
 * Parametri:
 * s: doppio puntatore all'oggetto Studente da deallocare
 *
 * Ritorna:
 * Nessuno (void).
 */
void studente_distruggi(Studente* s) {
    if (s != NULL && *s != NULL) {
        free(*s);
        *s = NULL; 
    }
}

/**
 * Fornisce l'accesso in sola lettura alla matricola dello studente.
 *
 * Garantisce l'information hiding pur permettendo ai sistemi esterni (come la
 * Tabella Hash dell'aula) di identificare univocamente l'utente.
 *
 * Parametri:
 * s: puntatore all'oggetto Studente da ispezionare
 *
 * Ritorna:
 * Il puntatore alla stringa della matricola, oppure NULL se lo studente non è valido.
 */
char* studente_ottieni_matricola(Studente s) {
    if (s == NULL) {
        return NULL;
    }
    return s->matricola;
}

/**
 * Output formattato a video dei dettagli anagrafici dello studente.
 *
 * Delega la formattazione grafica a questa funzione interna per mantenere
 * pulito il codice dei report e delle stampe dell'aula.
 *
 * Parametri:
 * s: puntatore all'oggetto Studente da mostrare
 *
 * Ritorna:
 * Nessuno (void).
 */
void studente_stampa(Studente s) {
    if (s != NULL) {
        printf("Matricola: %s, Nome: %s, Corso: %s\n", s->matricola, s->nome, s->corso);
    }
}

