#include <stdio.h>
#include <stdlib.h>
#include "aula.h"
#include "studente.h"


int main(){
	//creo l'aula con 5 posti per fare i test
	Aula mia_aula = aula_crea("Aula Studio Archimede", 5);
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


			 printf("Inserisci matricola (numerica): ");
			 scanf("%s", matricola);

			 printf("Inserisci nome (senza spazi): ");
			 scanf("%s", nome);

			 printf("Inserisci il corso di laurea (senza spazi): ");
			 scanf("%s", corso);


			 Studente s = studente_crea(matricola, nome, corso);

				if (s == NULL){
					printf("Errore: impossibile creare lo studente (memoria piena o dati non validi.)\n");
					break;
					}

				int esito = aula_ingresso(mia_aula, s);

				if (esito == 1){
					printf(">>> SUCCESSO: Lo studente %s e' entrato e si e' seduto.\n", nome);

					} else if (esito == 2){
						printf(">>> AULA PIENA: Lo studente %s e' stato messo in lista d'attesa.\n", nome);
						} else {
							printf(">>> ERRORE: Si e' verificato un problema durante l'ingresso.\n");
							}

		  	 break;
			}

		case 2: {
			 printf("Hai scelto: Uscita studente.\n");
			 printf("\n--- USCITA STUDENTE ---\n");
			 char matricola_out[20];

			 printf("Inserisci la matricola dello studente che deve uscire: ");
			 scanf("%s", matricola_out);

			 Studente s_out = studente_crea(matricola_out, "N/A", "N/A"); // utilizzo "N/A"(non applicabile) perche' sono valori che non servono alla funzione


			 int esito = aula_uscita(mia_aula, s_out);
				if(esito == 1){
					printf(">>> SUCCESSO: Lo studente con matricola %s e' uscito.\n", matricola_out);
					printf(">>> (Controlla lo stato per vedere se qualcuno in coda ha preso il suo posto)\n");
					} else{
						printf(">>> ERRORE: Studente con matricola %s non trovato in aula.\n", matricola_out);
						}


			 break;

			}


		case 3:
			 aula_stampa_stato(mia_aula);
			 break;



		case 4:
			 printf("Funzionalita' in sviluppo\n");
			 break;


		case 0:
			 printf("Chiusura del programma. Arrivederci.\n");
			 break;



		default:
			 printf("Scelta non valida. Riprova.\n");


		}






	 }while (scelta !=0);


 return 0;

}
