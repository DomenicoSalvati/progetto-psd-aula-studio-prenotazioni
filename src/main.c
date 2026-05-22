/**
 * File: main.c
 * Autore: Mathias
 * Data: Maggio 2026
 *
 * Descrizione: Programma principale e Interfaccia Utente (CLI) per la gestione
 * dell'aula studio. Offre un menù interattivo per gli ingressi diretti e un
 * sottomenù dedicato alla gestione avanzata delle prenotazioni a fasce orarie.
 */

#include <stdio.h>
#include <stdlib.h>
#include "aula.h"
#include "studente.h"
#include "prenotazione.h"

/**
 * Funzione principale del programma.
 *
 * Inizializza l'istanza dell'aula e avvia il ciclo di vita dell'interfaccia
 * utente a linea di comando. Gestisce l'allocazione e garantisce la
 * distruzione sicura della memoria al termine dell'esecuzione.
 *
 * Parametri:
 * Nessuno.
 *
 * Ritorna:
 * 0 al termine di un'esecuzione priva di errori.
 */
int main() {
    // Inizializzazione dell'aula con 50 posti per simulare un carico realistico
    Aula mia_aula = aula_crea("Aula Studio Archimede", 50);
    if (mia_aula == NULL) {
        printf("Errore critico: impossibile creare l'aula. Memoria insufficiente.\n");
        return -1;
    }

    int scelta;
    do {
        printf("\n=== GESTORE AULA STUDIO ===\n");
        printf("1. Ingresso studente (senza prenotazione)\n");
        printf("2. Uscita studente\n");
        printf("3. Stampa stato Aula e lista d'attesa\n");
        printf("4. Gestione Prenotazioni\n");
        printf("0. Esci\n");
        printf("Scegli un'opzione: ");

        // Validazione dell'input per prevenire loop causati da caratteri non numerici
        if (scanf("%d", &scelta) != 1) {
            while (getchar() != '\n');
            scelta = -1;
        }

        switch (scelta) {
            case 1: {
                char matricola[20];
                char nome[50];
                char corso[50];
                int posto_scelto;

                printf("\n--- INGRESSO STUDENTE ---\n");

                printf("Inserisci matricola (alfanumerica e max 19 caratteri): ");
                scanf("%19s", matricola);

                if (aula_studente_esiste(mia_aula, matricola)) {
                    printf(">>> ERRORE: Ingresso rifiutato. Lo studente con matricola %s e' gia' presente in aula.\n", matricola);
                    break;
                }

                printf("Inserisci nome (senza spazi e max 49 caratteri): ");
                scanf("%49s", nome);

                printf("Inserisci il corso di laurea (senza spazi e max 49 caratteri): ");
                scanf("%49s", corso);

                printf("Inserisci il posto desiderato (da 0 a 49): ");
                if (scanf("%d", &posto_scelto) != 1) {
                    while (getchar() != '\n');
                    printf(">>> ERRORE: Input non valido per il posto.\n");
                    break;
                }

                Studente s = studente_crea(matricola, nome, corso);
                if (s == NULL) {
                    printf(">>> ERRORE: Impossibile creare lo studente (memoria satura o dati non validi).\n");
                    break;
                }

                int esito = aula_ingresso(mia_aula, s, posto_scelto);

                if (esito == 1) {
                    printf(">>> SUCCESSO: Lo studente %s si e' seduto al posto %d.\n", nome, posto_scelto);
                } else if (esito == 2) {
                    printf(">>> AULA/POSTO OCCUPATO: Lo studente %s e' stato messo in lista d'attesa.\n", nome);
                } else {
                    printf(">>> ERRORE: Il posto inserito risulta fuori dai limiti consentiti dalla struttura.\n");
                    studente_distruggi(&s);
                }

                break;
            }

            case 2: {
                char matricola_uscente[20];

                printf("\n--- USCITA STUDENTE ---\n");
                printf("Inserisci la matricola dello studente uscente (max 19 caratteri): ");
                scanf("%19s", matricola_uscente);

                int esito = aula_uscita(mia_aula, matricola_uscente);

                if (esito == 1) {
                    printf(">>> SUCCESSO: Studente con matricola %s rimosso con successo.\n", matricola_uscente);
                    printf(">>> (Lo stato dell'aula e' stato aggiornato, eventuale coda processata).\n");
                } else {
                    printf(">>> ERRORE: Studente con matricola %s non trovato nel sistema.\n", matricola_uscente);
                }

                break;
            }

            case 3: {
                aula_stampa_stato(mia_aula);
                break;
            }

            case 4: {
                int scelta_prenotazione;
                do {
                    printf("\n=== SOTTOMENU GESTIONE PRENOTAZIONI ===\n");
                    printf("1. Inserisci nuova prenotazione\n");
                    printf("2. Effettua Check-in studente prenotato\n");
                    printf("3. Annulla prenotazione esistente\n");
                    printf("4. Stampa report statistico e storico\n");
                    printf("0. Torna al menu principale\n");
                    printf("Scegli un'opzione: ");

                    if (scanf("%d", &scelta_prenotazione) != 1) {
                        while (getchar() != '\n');
                        scelta_prenotazione = -1;
                    }

                    switch (scelta_prenotazione) {
                        case 1: {
                            char mat_pren[20], nome_pren[50], corso_pren[50];
                            char data_pren[15], fascia_pren[30];
                            int posto_prenotato;

                            printf("\n--- NUOVA PRENOTAZIONE ---\n");
                            printf("Inserisci matricola (max 19 caratteri): ");
                            scanf("%19s", mat_pren);

                            if (aula_studente_esiste(mia_aula, mat_pren)) {
                                printf(">>> ERRORE: Prenotazione rifiutata. Lo studente con matricola %s e' gia' registrato.\n", mat_pren);
                                break;
                            }

                            printf("Inserisci nome (max 49 caratteri): ");
                            scanf("%49s", nome_pren);

                            printf("Inserisci corso (max 49 caratteri): ");
                            scanf("%49s", corso_pren);

                            printf("Inserisci data (GG/MM/AAAA, max 14 caratteri): ");
                            scanf("%14s", data_pren);

                            // Intercettazione e valutazione della coerenza cronologica dell'anno inserito
                            if (valida_data(data_pren)) {
                                int anno_inserito = atoi(&data_pren[6]);
                                if (anno_inserito < 2026) {
                                    printf(">>> AVVISO CRONOLOGICO: L'anno %d inserito fa riferimento al passato.\n", anno_inserito);
                                } else if (anno_inserito > 2030) {
                                    printf(">>> AVVISO CRONOLOGICO: L'anno %d inserito risulta eccessivamente futuristico.\n", anno_inserito);
                                }
                            } else {
                                printf(">>> ERRORE: Formato data non valido. Rispettare il formato GG/MM/AAAA.\n");
                                break;
                            }

                            printf("Inserisci fascia oraria (es. 09:00-11:00, max 29 caratteri): ");
                            scanf("%29s", fascia_pren);
                            if (!valida_fascia(fascia_pren)) {
                                printf(">>> ERRORE: Formato fascia non valido o illogico. Rispettare HH:MM-HH:MM.\n");
                                break;
                            }

                            printf("Inserisci il posto da riservare (da 0 a 49): ");
                            if (scanf("%d", &posto_prenotato) != 1) {
                                while (getchar() != '\n');
                                printf(">>> ERRORE: Input numerico non valido per il posto.\n");
                                break;
                            }

                            Studente stud = studente_crea(mat_pren, nome_pren, corso_pren);
                            Prenotazione pren = prenotazione_crea(stud, data_pren, fascia_pren, posto_prenotato);

                            // La condizione valuta allocazione e avvenuta validazione senza conflitti
                            if (pren != NULL && aula_aggiungi_prenotazione(mia_aula, pren)) {
                                printf(">>> SUCCESSO: Prenotazione registrata in stato 'IN ATTESA'.\n");
                            } else {
                                printf(">>> ERRORE: La sedia %d risulta occupata da un'altra pratica per la medesima fascia oraria.\n", posto_prenotato);
                                if (pren != NULL) {
                                    prenotazione_distruggi(&pren);
                                    studente_distruggi(&stud);
                                }
                            }
                            break;
                        }

                        case 2: {
                            char mat_checkin[20];

                            printf("\n--- EFFETTUA CHECK-IN ---\n");
                            printf("Inserisci la matricola dello studente (max 19 caratteri): ");
                            scanf("%19s", mat_checkin);

                            int risultato = aula_checkin_prenotazione(mia_aula, mat_checkin);

                            if (risultato == 1) {
                                printf(">>> CHECK-IN OK: Studente convalidato e seduto correttamente al suo posto.\n");
                            } else if (risultato == 2) {
                                printf(">>> CHECK-IN OK: Posto attualmente occupato, studente inserito in coda d'attesa.\n");
                            } else if (risultato == -1) {
                                printf(">>> AVVISO: Check-in gia' effettuato in precedenza.\n");
                            } else {
                                printf(">>> ERRORE: Nessuna prenotazione pendente trovata per la matricola indicata.\n");
                            }
                            break;
                        }

                        case 3: {
                            char mat_annulla[20];

                            printf("\n--- ANNULLA PRENOTAZIONE ---\n");
                            printf("Inserisci la matricola della pratica da annullare (max 19 caratteri): ");
                            scanf("%19s", mat_annulla);

                            if (aula_annulla_prenotazione(mia_aula, mat_annulla)) {
                                printf(">>> SUCCESSO: Prenotazione rimossa definitivamente dal sistema.\n");
                            } else {
                                printf(">>> ERRORE: Impossibile annullare (pratica inesistente o studente gia' entrato).\n");
                            }
                            break;
                        }

                        case 4: {
                            aula_stampa_report_prenotazioni(mia_aula);
                            break;
                        }

                        case 0: {
                            printf("Ritorno al menu principale...\n");
                            break;
                        }

                        default: {
                            printf(">>> ERRORE: Opzione non valida nel sottomenu.\n");
                            break;
                        }
                    }

                } while (scelta_prenotazione != 0);

                break;
            }

            case 0: {
                printf("Chiusura del programma. Grazie per aver utilizzato il sistema.\n");
                break;
            }

            default: {
                printf(">>> ERRORE: Scelta non valida. Riprova con un tasto numerico (1, 2, 3, 4, 0).\n");
                break;
            }
        }

    } while (scelta != 0);

    // Deallocazione profonda prima della terminazione del processo
    aula_distruggi(&mia_aula);

    return 0;
}
