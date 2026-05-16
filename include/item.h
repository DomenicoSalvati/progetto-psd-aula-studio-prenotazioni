/**
 * File: item.h
 * Autore: Domenico
 * Data: Aprile 2026
 *
 * Descrizione: Definizione del tipo generico 'item' utilizzato come
 * elemento base per le strutture dati del sistema (es. Coda).
 * Mappa il tipo generico sull'ADT Studente.
 */

#ifndef ITEM_H //Se non è definita la macro corrispondente ad item.h, definisce item.h.
#define ITEM_H

#include "studente.h"

/**
 * Specifica sintattica:
 * typedef Studente item;
 *
 * Specifica semantica:
 * Definisce l'alias 'item' per il tipo 'Studente'. Questo livello di astrazione
 * permette di riutilizzare la struttura Coda con qualsiasi altro tipo di dato
 * in futuro, modificando solo questa riga.
 *
 * Pre-condizioni:
 * Il tipo 'Studente' deve essere visibile e incluso.
 *
 * Post-condizioni:
 * 'item' diventa un sinonimo valido del tipo 'Studente'.
 */
typedef Studente item;

/**
 * Costante che definisce il valore nullo o non valido per il tipo generico item.
 */
#define NULLITEM NULL

#endif
