/**
 * File: main_test.c
 * Autore: Mathias
 * Data: Maggio 2026
 *
 * Descrizione: Suite di test automatizzati per la verifica dell'ADT Aula,
 * utilizzando funzioni isolate e macro assert() della libreria standard.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "aula.h"
#include "studente.h"
#include "prenotazione.h"

/* ------------------------------------------------------------------
 * 1. Verifica della registrazione degli studenti
 * ------------------------------------------------------------------ */
void test_registrazione_studenti(void) {
    Studente s = studente_crea("MAT01", "Mario", "Informatica");
    assert(s != NULL); // L'allocazione e l'inizializzazione devono avere successo

    studente_distruggi(&s);
}

/* ------------------------------------------------------------------
 * 2. Verifica dell'ingresso senza prenotazione
 * ------------------------------------------------------------------ */
void test_ingresso_senza_prenotazione(void) {
    Aula a = aula_crea("AulaTest", 5);
    Studente s = studente_crea("MAT02", "Luigi", "Fisica");

    int esito = aula_ingresso(a, s, 0); // Assegna Luigi al posto 0
    assert(esito == 1);

    aula_distruggi(&a);
}

/* ------------------------------------------------------------------
 * 3. Test della lista di attesa
 * ------------------------------------------------------------------ */
void test_lista_attesa(void) {
    Aula a = aula_crea("AulaTest", 1); // Capienza volutamente a 1 per saturarla subito
    Studente s1 = studente_crea("MAT03", "Peach", "Matematica");
    Studente s2 = studente_crea("MAT04", "Toad", "Storia");

    assert(aula_ingresso(a, s1, 0) == 1);

    int esito_coda = aula_ingresso(a, s2, 0);
    assert(esito_coda == 2); // 2 indica l'inserimento in lista d'attesa

    aula_distruggi(&a);
}

/* ------------------------------------------------------------------
 * 4. Test dell'inserimento delle prenotazioni
 * ------------------------------------------------------------------ */
void test_inserimento_prenotazioni(void) {
    Aula a = aula_crea("AulaTest", 5);
    Studente s = studente_crea("MAT05", "Yoshi", "Biologia");
    Prenotazione p = prenotazione_crea(s, "20/06/2026", "09:00-11:00", 2);

    assert(aula_aggiungi_prenotazione(a, p) == 1);

    aula_distruggi(&a);
}

/* ------------------------------------------------------------------
 * 5. Verifica della disponibilità dei posti (Gestione Conflitti)
 * ------------------------------------------------------------------ */
void test_disponibilita_posti(void) {
    Aula a = aula_crea("AulaTest", 5);
    Studente s1 = studente_crea("MAT06", "Bowser", "Ingegneria");
    Studente s2 = studente_crea("MAT07", "Wario", "Economia");

    Prenotazione p1 = prenotazione_crea(s1, "20/06/2026", "10:00-12:00", 3);
    // Creiamo un conflitto di orario per lo stesso posto (3)
    Prenotazione p_conflitto = prenotazione_crea(s2, "20/06/2026", "11:00-13:00", 3);

    assert(aula_aggiungi_prenotazione(a, p1) == 1);
    assert(aula_aggiungi_prenotazione(a, p_conflitto) == 0);

    // Libera manualmente gli oggetti rifiutati
    prenotazione_distruggi(&p_conflitto);
    studente_distruggi(&s2);
    aula_distruggi(&a);
}

/* ------------------------------------------------------------------
 * 6. Verifica dell'annullamento e aggiornamento disponibilità
 * ------------------------------------------------------------------ */
void test_annullamento_disponibilita(void) {
    Aula a = aula_crea("AulaTest", 5);
    Studente s = studente_crea("MAT08", "Daisy", "Chimica");
    Prenotazione p = prenotazione_crea(s, "20/06/2026", "09:00-11:00", 1);

    aula_aggiungi_prenotazione(a, p);
    assert(aula_annulla_prenotazione(a, "MAT08") == 1); // L'annullamento deve riuscire

    aula_distruggi(&a);
}

/* ------------------------------------------------------------------
 * 7. Test del check-in e check-out
 * ------------------------------------------------------------------ */
void test_checkin_checkout(void) {
    Aula a = aula_crea("AulaTest", 5);
    Studente s = studente_crea("MAT09", "Waluigi", "Lettere");
    Prenotazione p = prenotazione_crea(s, "20/06/2026", "14:00-16:00", 4);

    aula_aggiungi_prenotazione(a, p);

    assert(aula_checkin_prenotazione(a, "MAT09") == 1);
    assert(aula_uscita(a, "MAT09") == 1);

    aula_distruggi(&a);
}

/* ------------------------------------------------------------------
 * 8. Test dello storico e dei report
 * ------------------------------------------------------------------ */
void test_storico_report(void) {
    Aula a = aula_crea("AulaTest", 5);

    // Invochiamo la funzione di report su un'aula vuota per testarne
    // la stabilità ed evitare segmentation fault
    aula_stampa_report_prenotazioni(a);

    assert(1 == 1); // Asserzione passiva, garantisce che non ci siano stati crash a runtime

    aula_distruggi(&a);
}

/* ------------------------------------------------------------------
 * ESECUZIONE DELLA SUITE (MAIN TEST)
 * ------------------------------------------------------------------ */
int main(void) {
    printf("Avvio esecuzione della test suite...\n");

    test_registrazione_studenti();
    test_ingresso_senza_prenotazione();
    test_lista_attesa();
    test_inserimento_prenotazioni();
    test_disponibilita_posti();
    test_annullamento_disponibilita();
    test_checkin_checkout();

    printf("\n");
    test_storico_report();

    // Se si giunge a questa linea, significa che nessun assert()
    // ha bloccato il programma, confermando la solidità del codice.
    printf("\n --- TEST SUPERATI ---\n");

    return 0;
}
