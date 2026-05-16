/**
 * File: studente.h
 * Autore: Mathias
 * Data: Aprile 2026
 *
 * Descrizione: Interfaccia per il Tipo di Dato Astratto (ADT) Studente.
 * Definisce la struttura opaca e le operazioni per creare, gestire
 * e distruggere le informazioni di un singolo studente universitario.
 */

#ifndef STUDENTE_H //Se non è definita la macro di studente.h allora definisci studente.h.
#define STUDENTE_H


/**
 * TIPO DI DATO ASTRATTO: Studente
 *
 * Implementato tramite puntatore opaco a struct per garantire l'information
 * hiding. Nasconde i dettagli interni (matricola, nome, corso) all'utente.
 */
typedef struct studente* Studente;


/**
 * Specifica sintattica:
 * Studente studente_crea(char* matricola, char* nome, char* corso);
 *
 * Specifica semantica:
 * Alloca dinamicamente la memoria per un nuovo studente e ne inizializza
 * i campi copiando le stringhe fornite.
 *
 * Pre-condizioni:
 * I parametri 'matricola', 'nome' e 'corso' devono essere stringhe valide
 * (non NULL) e correttamente terminate.
 *
 * Post-condizioni:
 * Viene creato un nuovo oggetto Studente. I valori interni sono copie
 * esatte dei parametri passati.
 *
 * Valore di ritorno:
 * Puntatore al nuovo studente creato. Restituisce NULL si uno dei
 * parametri è NULL o se fallisce l'allocazione della memoria.
 */
Studente studente_crea(char* matricola, char* nome, char* corso);


/**
 * Specifica sintattica:
 * void studente_distruggi(Studente* s);
 *
 * Specifica semantica:
 * Libera la memoria allocata dinamicamente per l'oggetto studente
 * e imposta il puntatore di origine a NULL per sicurezza.
 *
 * Pre-condizioni:
 * L'indirizzo 's' non deve essere NULL e '*s' deve puntare a un oggetto
 * studente valido allocato in precedenza.
 *
 * Post-condizioni:
 * La memoria puntata da '*s' viene interamente liberata nell'heap.
 * Il puntatore '*s' viene impostato a NULL per prevenire dangling pointers.
 *
 * Valore di ritorno:
 * Nessuno (void).
 */
void studente_distruggi(Studente* s);


/**
 * Specifica sintattica:
 * char* studente_ottieni_matricola(Studente s);
 *
 * Specifica semantica:
 * Fornisce l'accesso protetto in sola lettura alla stringa contenente
 * la matricola dello studente richiesto.
 *
 * Pre-condizioni:
 * Lo studente 's' deve essere un puntatore valido (non NULL).
 *
 * Post-condizioni:
 * Lo stato interno dello studente non subisce alcuna forma di modifica.
 *
 * Valore di ritorno:
 * Puntatore alla stringa della matricola. Restituisce NULL se lo studente
 * passato in ingresso non è valido (NULL).
 */
char* studente_ottieni_matricola(Studente s);


/**
 * Specifica sintattica:
 * void studente_stampa(Studente s);
 *
 * Specifica semantica:
 * Stampa a video i dettagli dello studente formattati in modo pulito
 * e leggibile (mostrando Matricola, Nome e Corso di Laurea).
 *
 * Pre-condizioni:
 * Lo studente 's' deve essere un puntatore valido e allocato (non NULL).
 *
 * Post-condizioni:
 * Lo stato dello studente rimane del tutto invariato. I dati vengono
 * formattati e inviati sul canale standard output.
 *
 * Valore di ritorno:
 * Nessuno (void).
 */
void studente_stampa(Studente s);


#endif
