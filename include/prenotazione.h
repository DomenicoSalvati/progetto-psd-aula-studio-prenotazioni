/**
 * File: prenotazione.h
 * Autore: Domenico
 * Data: Maggio 2026
 *
 * Descrizione: Interfaccia dell'ADT Prenotazione. Definisce il tipo opaco e
 * le operazioni per gestire le prenotazioni degli studenti, includendo data,
 * fascia oraria e lo stato di validazione (check-in).
 */

#ifndef PRENOTAZIONE_H // Se la macro di prenotazione.h non è definita, allora definisci prenotazione.h.
#define PRENOTAZIONE_H

#include "studente.h"

/**
 * TIPO DI DATO ASTRATTO: Prenotazione
 * * Implementato tramite puntatore a struct.
 */
typedef struct prenotazione* Prenotazione;

/**
 * Specifica sintattica:
 * Prenotazione prenotazione_crea(Studente s, char* data, char* fascia);
 *
 * Specifica semantica:
 * Alloca e inizializza un oggetto Prenotazione. Lo stato iniziale è
 * impostato di default a 0 (Prenotato / Da confermare).
 *
 * Pre-condizioni:
 * Il parametro 's' deve essere uno Studente valido. 'data' e 'fascia'
 * non devono essere stringhe NULL.
 *
 * Post-condizioni:
 * Viene creata una nuova prenotazione pronta per essere gestita dal sistema.
 *
 * Valore di ritorno:
 * Puntatore alla nuova Prenotazione, oppure NULL in caso di errore.
 */
Prenotazione prenotazione_crea(Studente s, char* data, char* fascia);

/**
 * Specifica sintattica:
 * void prenotazione_distruggi(Prenotazione* p);
 *
 * Specifica semantica:
 * Dealloca l'oggetto Prenotazione dalla memoria heap.
 *
 * Pre-condizioni:
 * p è un doppio puntatore valido a un oggetto Prenotazione.
 *
 * Post-condizioni:
 * La memoria è liberata e il puntatore viene settato a NULL. Lo studente
 * associato NON viene distrutto.
 *
 * Valore di ritorno:
 * Nessuno.
 */
void prenotazione_distruggi(Prenotazione* p);

/**
 * Specifica sintattica:
 * Studente prenotazione_ottieni_studente(Prenotazione p);
 *
 * Specifica semantica:
 * Restituisce il riferimento allo studente associato alla prenotazione.
 *
 * Pre-condizioni:
 * p è un oggetto Prenotazione valido.
 *
 * Post-condizioni:
 * Nessuna modifica all'oggetto.
 *
 * Valore di ritorno:
 * Puntatore all'oggetto Studente.
 */
Studente prenotazione_ottieni_studente(Prenotazione p);

/**
 * Specifica sintattica:
 * void prenotazione_effettua_checkin(Prenotazione p);
 *
 * Specifica semantica:
 * Aggiorna lo stato della prenotazione segnando che lo studente si è
 * effettivamente presentato in aula.
 *
 * Pre-condizioni:
 * p è un oggetto Prenotazione valido.
 *
 * Post-condizioni:
 * Lo stato interno della prenotazione passa a 1 (confermato).
 *
 * Valore di ritorno:
 * Nessuno.
 */
void prenotazione_effettua_checkin(Prenotazione p);

/**
 * Specifica sintattica:
 * int prenotazione_ottieni_stato(Prenotazione p);
 *
 * Specifica semantica:
 * Legge lo stato attuale della pratica.
 *
 * Pre-condizioni:
 * p è un oggetto valido.
 *
 * Post-condizioni:
 * Nessuna modifica.
 *
 * Valore di ritorno:
 * Intero rappresentante lo stato (0 = Solo prenotato, 1 = Check-in fatto), -1 in caso di errore.
 */
int prenotazione_ottieni_stato(Prenotazione p);

/**
 * Specifica sintattica:
 * void prenotazione_stampa(Prenotazione p);
 *
 * Specifica semantica:
 * Mostra su standard output i dettagli della prenotazione.
 *
 * Pre-condizioni:
 * p è un oggetto valido.
 *
 * Post-condizioni:
 * Nessuna modifica.
 *
 * Valore di ritorno:
 * Nessuno.
 */
void prenotazione_stampa(Prenotazione p);

#endif
