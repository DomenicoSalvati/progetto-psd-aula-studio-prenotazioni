/**
 * File: aula.h
 * Autori: Domenico e Mathias
 * Data: creato Aprile 2026, fine implementazioni Maggio 2026
 *
 * Descrizione: Interfaccia per l'ADT Aula. Definisce il tipo opaco e le
 * operazioni per gestire i posti a sedere, la coda d'attesa degli studenti,
 * la ricerca rapida tramite la Tabella Hash (Chaining) e le prenotazioni.
 */

#ifndef AULA_H // Se non è definita la macro di aula.h allora definisci aula.h.
#define AULA_H

#include "studente.h"
#include "prenotazione.h"

/**
 * TIPO DI DATO ASTRATTO: Aula
 *
 * Implementato tramite puntatore a struct.
 */
typedef struct aula* Aula;


/**
 * Specifica sintattica:
 * Aula aula_crea(char* nome, int capienza_massima);
 *
 * Specifica semantica:
 * Alloca dinamicamente la memoria per un nuovo oggetto Aula, configurando
 * l'array dei posti, la coda d'attesa, la tabella hash e il registro.
 *
 * Pre-condizioni:
 * Il parametro 'nome' deve essere una stringa valida (non NULL).
 * Il parametro 'capienza_massima' deve essere un intero maggiore di zero.
 *
 * Post-condizioni:
 * Viene creato un nuovo oggetto Aula inizializzato e pronto all'uso.
 *
 * Valore di ritorno:
 * Puntatore alla nuova Aula creata. Restituisce NULL in caso di errore.
 */
Aula aula_crea(char* nome, int capienza_massima);


/**
 * Specifica sintattica:
 * void aula_distruggi(Aula* a);
 *
 * Specifica semantica:
 * Rilascia l'intera memoria occupata dall'aula, distruggendo tutti gli oggetti
 * interni, la coda, la tabella hash e deallocando le prenotazioni.
 *
 * Pre-condizioni:
 * L'indirizzo 'a' non deve essere NULL e '*a' deve puntare a un'Aula valida.
 *
 * Post-condizioni:
 * Tutta la memoria associata all'aula viene liberata. Puntatore a NULL.
 *
 * Valore di ritorno:
 * Nessuno (void).
 */
void aula_distruggi(Aula* a);


/**
 * Specifica sintattica:
 * int aula_ingresso(Aula a, Studente s, int posto_richiesto);
 *
 * Specifica semantica:
 * Gestisce l'ingresso di uno studente assegnandogli un posto specifico nell'aula.
 *
 * Pre-condizioni:
 * L'oggetto 'a' e l'oggetto 's' devono essere puntatori validi (non NULL).
 * 'posto_richiesto' deve essere un intero compreso tra 0 e (capienza_massima - 1).
 *
 * Post-condizioni:
 * Lo studente viene inserito nel posto specifico a sedere. Se il posto e'
 * gia' occupato o l'aula e' piena, lo studente va nella coda d'attesa.
 *
 * Valore di ritorno:
 * 1 (seduto), 2 (in coda d'attesa), 0 (errore o posto non valido).
 */
int aula_ingresso(Aula a, Studente s, int posto_richiesto);


/**
 * Specifica sintattica:
 * int aula_uscita(Aula a, char* matricola_da_cercare);
 *
 * Specifica semantica:
 * Rimuove uno studente dall'aula identificandolo tramite la matricola in O(1).
 *
 * Pre-condizioni:
 * 'a' e 'matricola_da_cercare' non devono essere NULL.
 *
 * Post-condizioni:
 * Lo studente esce dall'aula, subentra nel medesimo posto il primo in coda.
 *
 * Valore di ritorno:
 * 1 (successo), 0 (studente non presente).
 */
int aula_uscita(Aula a, char* matricola_da_cercare);


/**
 * Specifica sintattica:
 * void aula_stampa_stato(Aula a);
 *
 * Specifica semantica:
 * Mostra sullo standard output lo stato corrente dell'aula.
 *
 * Pre-condizioni:
 * L'oggetto 'a' deve essere valido.
 *
 * Post-condizioni:
 * Nessuna modifica interna.
 *
 * Valore di ritorno:
 * Nessuno (void).
 */
void aula_stampa_stato(Aula a);

/**
 * Specifica sintattica:
 * int aula_aggiungi_prenotazione(Aula a, Prenotazione p);
 *
 * Specifica semantica:
 * Registra una nuova prenotazione nel sistema dell'aula.
 *
 * Pre-condizioni:
 * 'a' e 'p' puntatori validi.
 *
 * Post-condizioni:
 * La prenotazione viene inserita nel registro storico interno.
 *
 * Valore di ritorno:
 * 1 (successo), 0 (errore).
 */
int aula_aggiungi_prenotazione(Aula a, Prenotazione p);

/**
 * Specifica sintattica:
 * int aula_checkin_prenotazione(Aula a, char* matricola);
 *
 * Specifica semantica:
 * Convalida l'arrivo di uno studente prenotato facendolo fisicamente entrare
 * nel posto che aveva riservato.
 *
 * Pre-condizioni:
 * 'a' e 'matricola' validi.
 *
 * Post-condizioni:
 * Stato prenotazione aggiornato e studente smistato (posto fisico o coda).
 *
 * Valore di ritorno:
 * 1 (check-in fatto, seduto), 2 (check-in fatto, in coda), -1 (già entrato), 0 (non trovato).
 */
int aula_checkin_prenotazione(Aula a, char* matricola);

/**
 * Specifica sintattica:
 * int aula_annulla_prenotazione(Aula a, char* matricola);
 *
 * Specifica semantica:
 * Rimuove una prenotazione pendente e dealloca lo studente associato.
 *
 * Pre-condizioni:
 * 'a' e 'matricola' validi.
 *
 * Post-condizioni:
 * Prenotazione rimossa definitivamente dal sistema.
 *
 * Valore di ritorno:
 * 1 (successo), 0 (fallimento o già in aula).
 */
int aula_annulla_prenotazione(Aula a, char* matricola);

/**
 * Specifica sintattica:
 * void aula_stampa_report_prenotazioni(Aula a);
 *
 * Specifica semantica:
 * Genera il report statistico degli accessi e dei no-show.
 *
 * Pre-condizioni:
 * 'a' valido.
 *
 * Post-condizioni:
 * Nessuna.
 *
 * Valore di ritorno:
 * Nessuno.
 */
void aula_stampa_report_prenotazioni(Aula a);

/**
 * Specifica sintattica:
 * int aula_studente_esiste(Aula a, char* matricola);
 *
 * Specifica semantica:
 * Controlla se una matricola è attualmente seduta in aula tramite ricerca rapida.
 *
 * Pre-condizioni:
 * 'a' e 'matricola' validi.
 *
 * Post-condizioni:
 * Nessuna.
 *
 * Valore di ritorno:
 * 1 (se presente), 0 (se non presente).
 */
int aula_studente_esiste(Aula a, char* matricola);

#endif
