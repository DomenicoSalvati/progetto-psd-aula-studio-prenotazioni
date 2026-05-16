/**
 * File: prenotazione.c
 * Autore: Domenico
 * Data: Maggio 2026
 *
 * Descrizione: Implementazione concreta dell'ADT Prenotazione.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "prenotazione.h"

// Struttura dati nascosta per mantenere incapsulati i dettagli di data e ora.
struct prenotazione {
    Studente studente;
    char data[15]; // Si aspetta del tipo GG/MM/AAAA.
    char fascia_oraria[30]; // Si aspetta del tipo 09:00-11:00.
    int stato;
};

// Alloca e popola i campi della nuova prenotazione.
Prenotazione prenotazione_crea(Studente s, char* data, char* fascia) {
    if (s == NULL || data == NULL || fascia == NULL) {
        return NULL;
    }

    Prenotazione p = (Prenotazione) malloc(sizeof(struct prenotazione));
    if (p == NULL) return NULL;

    p->studente = s;

    strncpy(p->data, data, sizeof(p->data) - 1);
    p->data[sizeof(p->data) - 1] = '\0';

    strncpy(p->fascia_oraria, fascia, sizeof(p->fascia_oraria) - 1);
    p->fascia_oraria[sizeof(p->fascia_oraria) - 1] = '\0';

    p->stato = 0;

    return p;
}

// Libera la memoria del contenitore Prenotazione.
void prenotazione_distruggi(Prenotazione* p) {
    if (p != NULL && *p != NULL) {
        free(*p);
        *p = NULL;
    }
}

// Accesso in sola lettura al riferimento dello studente.
Studente prenotazione_ottieni_studente(Prenotazione p) {
    if (p == NULL) return NULL;
    return p->studente;
}

// Esegue il passaggio di stato per validare l'ingresso fisico in aula.
void prenotazione_effettua_checkin(Prenotazione p) {
    if (p != NULL) {
        p->stato = 1;
    }
}

// Restituisce l'indicatore di stato attuale della pratica.
int prenotazione_ottieni_stato(Prenotazione p) {
    if (p == NULL) return -1;
    return p->stato;
}

// Output formattato a video dei dati essenziali della prenotazione.
void prenotazione_stampa(Prenotazione p) {
    if (p != NULL) {
        printf("Data: %s | Fascia: %s | Stato: %s\n",
               p->data,
               p->fascia_oraria,
               (p->stato == 1) ? "CHECK-IN EFFETTUATO" : "IN ATTESA");
        printf("  -> ");
        studente_stampa(p->studente);
    }
}
