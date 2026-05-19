/*
*File: main.c
*Autore: Mathias
*Data: Maggio 2026
*
*Descrizione: Programma principale (Interfaccia Utente) per la gestione
*dell'aula studio. Offre menù principale per gli ingressi diretti e un
*sottomenù dedicato alla gestione completa delle prenotazioni a fasce orarie.
*/


#include <stdio.h>
#include <stdlib.h>
#include "aula.h"
#include "studente.h"
#include "prenotazione.h"


int main(){
	//creo l'aula con 50 posti per simulare il carico reale di un'aula studio universitaria
	Aula mia_aula = aula_crea("Aula Studio Archimede", 50);
	  if(mia_aula == NULL){
		printf("Errore: impossibile creare l'aula.\n");
		return -1;
	   }

	int scelta;
	do{
	   printf("\n=== GESTORE AULA STUDIO ===\n");
	   printf("1. Ingresso studente (senza prenotazione)\n");
	   printf("2. Uscita studente\n");
	   printf("3. Stampa stato Aula e lista d'attesa\n");
	   printf("4. Gestione Prenotazioni\n");
	   printf("0. Esci\n");
	   printf("Scegli un'opzione: ");

		// La validazione del ritorno di scanf previene loop infiniti se l'utente inserisce caratteri non numerici
		if(scanf("%d", &scelta) !=1){
			while(getchar() != '\n'); //pulisce il buffer se l'utente inserisce una lettera
			scelta = -1;
		   }

	switch(scelta){
		case 1: {
			 printf("Hai scelto ingresso studente.\n");
			 printf("\n--- INGRESSO STUDENTE ---\n");
			 char matricola[20];
			 char nome[50];
			 char corso[50];


			 printf("Inserisci matricola (alfanumerica e max 19 caratteri): ");
			 scanf("%19s", matricola);

			 printf("Inserisci nome (senza spazi e max 49 caratteri): ");
			 scanf("%49s", nome);

			 printf("Inserisci il corso di laurea (senza spazi e max 49 caratteri): ");
			 scanf("%49s", corso);


			 Studente s = studente_crea(matricola, nome, corso);

				if (s == NULL){
					printf("Errore: impossibile creare lo studente (memoria piena o dati non validi.)\n");
					break;
					}

				int esito = aula_ingresso(mia_aula, s);

				if (esito == 1){
					printf(">>> SUCCESSO: Lo studente %s è entrato e si è seduto.\n", nome);

					} else if (esito == 2){
						printf(">>> AULA PIENA: Lo studente %s è stato messo in lista d'attesa.\n", nome);
						} else {
							printf(">>> ERRORE: Si è verificato un problema durante l'ingresso.\n");
							}

		  	 break;
			}

		case 2: {
			 printf("Hai scelto: Uscita studente.\n");
			 printf("\n--- USCITA STUDENTE ---\n");
			 char matricola_uscente[20];

			 printf("Inserisci la matricola dello studente che deve uscire (max 19 caratteri): ");
			 scanf("%19s", matricola_uscente);



			 int esito = aula_uscita(mia_aula, matricola_uscente);
				if(esito == 1){
					printf(">>> SUCCESSO: Lo studente con matricola %s è uscito.\n", matricola_uscente);
					printf(">>> (Controlla lo stato per vedere se qualcuno in coda ha preso il suo posto)\n");
					} else{
						printf(">>> ERRORE: Studente con matricola %s non trovato in aula.\n", matricola_uscente);
						}


			 break;

			}


		case 3:
			 aula_stampa_stato(mia_aula);
			 break;



		case 4: {
			 int scelta_prenotazione;
			 do{
			     printf("\n=== SOTTOMENU GESTIONE PRENOTAZIONI ===\n");
			     printf("1. Inserisci nuova prenotazione\n");
			     printf("2. Effettua Check-in studente prenotato\n");
			     printf("3. Annulla prenotazione esistente\n");
			     printf("4. Stampa report statistico e storico\n");
			     printf("0. Torna al menu principale\n");
			     printf("Scegli un'opzione: ");

				if(scanf("%d", &scelta_prenotazione) != 1) {
					 while (getchar() != '\n');
					 scelta_prenotazione = -1;
					}

				switch(scelta_prenotazione){
					case 1: {
					     char mat_pren[20], nome_pren[50], corso_pren[50];
					     char data_pren[15], fascia_pren[30];

					     printf("\n--- NUOVA PRENOTAZIONE ---\n");
					     printf("Inserisci matricola (max 19 caratteri): ");
					     scanf("%19s", mat_pren);
					     printf("Inserisci nome(max 49 caratteri): ");
					     scanf("%49s", nome_pren);
					     printf("Inserisci corso(max 49 caratteri): ");
					     scanf("%49s", corso_pren);

					     printf("Inserisci data (GG/MM/AAAA, max 14 caratteri): ");
					     scanf("%14s", data_pren);
						if(!valida_data(data_pren)){
							printf(">>> ERRORE: Formato non valido. Deve essere GG/MM/AAAA.\n");
							break;
								}

					     printf("Inserisci fascia oraria (es. 09:00-11:00, max 29 caratteri): ");
					     scanf("%29s", fascia_pren);
						if(!valida_fascia(fascia_pren)){
							printf(">>> ERRORE: Formato fascia oraria non valido. Deve essere HH:MM-HH:MM.\n");
							break;
								}


					     Studente stud = studente_crea(mat_pren, nome_pren, corso_pren);
					     Prenotazione pren = prenotazione_crea(stud, data_pren, fascia_pren);

						// La condizione verifica contemporaneamente la corretta allocazione e l'avvenuto inserimento nel registro
						if(pren != NULL && aula_aggiungi_prenotazione(mia_aula, pren)) {
							printf(">>> SUCCESSO: Prenotazione registrata in stato 'IN ATTESA'.\n");
						}else {
							printf(">>> ERRORE: Impossibile registrare la prenotazione.\n");
							}
						 break;
						}

					case 2: {
					     char mat_checkin[20];
					     printf("\n--- EFFETTUA CHECK-IN ---\n");
					     printf("Inserisci la matricola dello studente(max 19 caratteri): ");
					     scanf("%19s", mat_checkin);

					     int risultato = aula_checkin_prenotazione(mia_aula, mat_checkin);
						if(risultato == 1){
							printf(">>> CHECK-IN OK: Studente seduto correttamente.\n");
						}else if(risultato == 2){
							printf(">>> CHECK-IN OK:Aula satura, studente in coda d'attesa.\n");
							}else if(risultato == -1){
							   printf(">>> AVVISO: Check-in già effettutato.\n");
								}else {
								   printf(">>> ERRORE: Nessuna prenotazione pendente trovata.\n");
									}

						 break;
						}

					case 3: {
					     char mat_annulla[20];
					     printf("\n--- ANNULLA PRENOTAZIONE ---\n");
					     printf("Inserisci la matricola da annullare(max 19 caratteri): ");
					     scanf("%19s", mat_annulla);

						// Sfrutta la valutazione booleana del C: procede solo se la funzione restituisce un valore diverso da 0
						if(aula_annulla_prenotazione(mia_aula, mat_annulla)){
							printf(">>> SUCCESSO: Prenotazione annullata.\n");
							}else{
							  printf(">>> ERRORE: Impossibile annullare (inesistente o studente già entrato).\n");
							  }
						 break;
						}

					case 4:
					     aula_stampa_report_prenotazioni(mia_aula);
						 break;

					case 0:
					     printf("Ritorno al menu principale...\n");
						 break;

					default:
					     printf("Opzione non valida\n");
						 break;
						}


				}while(scelta_prenotazione != 0);

			 break;
			}


		case 0:
			 printf("Chiusura del programma. Grazie per aver utilizzato il nostro gestore. Arrivederci.\n");
			 break;



		default:
			 printf("Scelta non valida. Riprova con un tasto numerico tra quelli proposti (1, 2, 3, 4, 0).\n");


		}






	 }while (scelta !=0);
	// Deallocazione profonda necessaria per prevenire memory leak prima della terminazione del processo
	aula_distruggi(&mia_aula);
	printf("Memoria ripulita correttamente. Arrivederci\n");

 return 0;

}
