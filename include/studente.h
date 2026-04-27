#ifndef STUDENTE_H
#define STUDENTE_H

/**
 * SPECIFICA SINTATTICA ADT STUDENTE
 * Tipo di dato astratto per rappresentare uno studente dell'Ateneo.
 * Implementato tramite puntatore a struct per nascondere i dettagli (Information Hiding).
 */
typedef struct studente* Studente;

/**
 * SPECIFICA SEMANTICA: studente_crea
 * Sintassi: studente_crea(char* matricola, char* nome, char* corso) -> Studente
 * Semantica: Alloca memoria per un nuovo studente e ne inizializza i campi.
 * Pre-condizioni: matricola, nome e corso non devono essere NULL.
 * Post-condizioni: Restituisce il puntatore allo studente creato. Se l'allocazione fallisce, restituisce NULL.
 */
Studente studente_crea(char* matricola, char* nome, char* corso);

/**
 * SPECIFICA SEMANTICA: studente_distruggi
 * Sintassi: studente_distruggi(Studente* s)
 * Semantica: Libera la memoria occupata dallo studente e imposta il puntatore a NULL per evitare "dangling pointers".
 * Pre-condizioni: s non deve essere NULL e *s deve puntare a uno studente valido.
 */
void studente_distruggi(Studente* s);

/**
 * SPECIFICA SEMANTICA: studente_get_matricola
 * Sintassi: studente_get_matricola(Studente s) -> char*
 * Semantica: Restituisce la stringa della matricola.
 * Pre-condizioni: s non deve essere NULL.
 */
char* studente_get_matricola(Studente s);

/**
 * SPECIFICA SEMANTICA: studente_stampa
 * Sintassi: studente_stampa(Studente s)
 * Semantica: Stampa i dati dello studente nel formato: [Matricola] Nome - Corso.
 * Pre-condizioni: s non deve essere NULL.
 */
void studente_stampa(Studente s);

#endif
