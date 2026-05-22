/**
 * File: prenotazione.h
 * Autore: Domenico
 * Data: Maggio 2026
 *
 * Descrizione: Interfaccia dell'ADT Prenotazione. Definisce il tipo opaco e
 * le operazioni per gestire le prenotazioni degli studenti, includendo data,
 * fascia oraria, posto assegnato e lo stato di validazione (check-in).
 */

#ifndef PRENOTAZIONE_H //Se non è definita la macro in prenotazione.h, allora definisci prenotazione.h.
#define PRENOTAZIONE_H

#include "studente.h"

/**
 * TIPO DI DATO ASTRATTO: Prenotazione
 * * Implementato tramite puntatore a struct.
 */
typedef struct prenotazione* Prenotazione;

/**
 * Specifica sintattica:
 * Prenotazione prenotazione_crea(Studente s, char* data, char* fascia, int posto_scelto);
 *
 * Specifica semantica:
 * Alloca e inizializza un oggetto Prenotazione assegnando studente, data, 
 * fascia oraria e il numero di sedia specifico scelto.
 *
 * Pre-condizioni:
 * Il parametro 's' deve essere uno Studente valido. 'data' e 'fascia'
 * non devono essere stringhe NULL. 'posto_scelto' deve essere valido.
 *
 * Post-condizioni:
 * Viene creata una nuova prenotazione pronta per essere gestita dal sistema.
 *
 * Valore di ritorno:
 * Puntatore alla nuova Prenotazione, oppure NULL in caso di errore.
 */
Prenotazione prenotazione_crea(Studente s, char* data, char* fascia, int posto_scelto);

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
 * int prenotazione_ottieni_posto(Prenotazione p);
 *
 * Specifica semantica:
 * Restituisce il numero del posto a sedere riservato durante la prenotazione.
 *
 * Pre-condizioni:
 * p è un oggetto valido.
 *
 * Post-condizioni:
 * Nessuna modifica.
 *
 * Valore di ritorno:
 * Intero indicante il numero di posto. Restituisce -1 in caso di errore.
 */
int prenotazione_ottieni_posto(Prenotazione p);

/**
 * Specifica sintattica:
 * char* prenotazione_ottieni_data(Prenotazione p);
 *
 * Specifica semantica:
 * Restituisce la stringa contenente la data della prenotazione.
 *
 * Pre-condizioni:
 * p è un oggetto valido.
 *
 * Post-condizioni:
 * Nessuna modifica all'oggetto.
 *
 * Valore di ritorno:
 * Puntatore alla stringa della data.
 */
char* prenotazione_ottieni_data(Prenotazione p);

/**
 * Specifica sintattica:
 * char* prenotazione_ottieni_fascia(Prenotazione p);
 *
 * Specifica semantica:
 * Restituisce la stringa contenente la fascia oraria della prenotazione.
 *
 * Pre-condizioni:
 * p è un oggetto valido.
 *
 * Post-condizioni:
 * Nessuna modifica all'oggetto.
 *
 * Valore di ritorno:
 * Puntatore alla stringa della fascia oraria.
 */
char* prenotazione_ottieni_fascia(Prenotazione p);

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

/**
 * Specifica sintattica:
 * int valida_data(const char* data);
 *
 * Specifica semantica:
 * Verifica che la stringa passata come parametro rispetti
 * il formato di una data espressa come "GG/MM/AAAA".
 *
 * Pre-condizioni:
 * Il parametro 'data' deve essere un puntatore a una stringa valida,
 * correttamente terminata dal carattere '\0' e non deve essere NULL.
 *
 * Post-condizioni:
 * Lo stato del sistema e la stringa originale 'data' rimangono inalterati.
 *
 * Valori di ritorno:
 * 1 se rispetta il formato, 0 se la lunghezza o i caratteri non sono corretti.
 */
int valida_data(const char* data);

/**
 * Specifica sintattica:
 * int valida_fascia(const char* fascia);
 *
 * Specifica semantica:
 * Verifica che la stringa passata come parametro rispetti
 * il formato di una fascia oraria espressa come "HH:MM-HH:MM".
 *
 * Pre-condizioni:
 * Il parametro 'fascia' deve essere un puntatore a una stringa valida,
 * correttamente terminata dal carattere '\0' e non deve essere NULL.
 *
 * Post-condizioni:
 * Lo stato del sistema e la stringa originale 'fascia' rimangono inalterati.
 *
 * Valori di ritorno:
 * 1 se rispetta il formato, 0 se non corretto o incongruente logicamente.
 */
int valida_fascia(const char* fascia);

#endif
