#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "aula.h"
#include "coda.h"

struct aula {
	char nome[100];
	int capienza_massima;
	int posti_occupati;
	Studente* posti;       // array dinamico per i posti fisici
	Coda lista_attesa;     // ADT Coda per chi non trova posto
};

Aula aula_crea(char* nome, int capienza_massima){
	if (nome == NULL || capienza_massima <= 0) return NULL;

	Aula a = (Aula) malloc(sizeof(struct aula));
	if (a == NULL) return NULL;

	strncpy(a->nome, nome, sizeof(a->nome)-1);
	a->nome[sizeof(a->nome)-1] = '\0';
	a->capienza_massima = capienza_massima;
	a->posti_occupati = 0;

	// inizializzazione delle strutture dati interne
	a->posti = (Studente*) calloc(capienza_massima, sizeof(Studente));
	a->lista_attesa = coda_crea();

	// se una delle due allocazioni fallisce, libero tutto per evitare memory leak
	if (a->posti == NULL || a->lista_attesa == NULL){
		if (a->posti) free(a->posti);
		if (a->lista_attesa) coda_distruggi(&(a->lista_attesa));
		free(a);
		return NULL;
	}

	return a;
}

void aula_distruggi(Aula* a){
	if (a != NULL && *a != NULL){
		if ((*a)->posti != NULL) free((*a)->posti);

		// svuota e dealloca passando il riferimento
		coda_distruggi(&((*a)->lista_attesa));

		free(*a);
		*a = NULL;
	}
}

int aula_ingresso(Aula a, Studente s){
	if (a == NULL || s == NULL) return 0;

	// ricerca di un posto fisico disponibile
	if (a->posti_occupati < a->capienza_massima){
		for (int i = 0; i < a->capienza_massima; i++){
			if (a->posti[i] == NULL){
				a->posti[i] = s;
				a->posti_occupati++;
				return 1;
			}
		}
	}

	// se l'aula è piena, lo studente entra nella lista d'attesa
	if (coda_inserisci(s, a->lista_attesa)){
		return 2;
	}

	return 0;
}

int aula_uscita(Aula a, char* matricola_da_cercare){
	if (a == NULL || matricola_da_cercare  == NULL) return 0;

	for (int i = 0; i < a->capienza_massima; i++){
		if (a->posti[i] != NULL && strcmp(studente_ottieni_matricola(a->posti[i]), matricola_da_cercare) == 0){
			// libera il posto dello studente uscente
			free(a->posti[i]);
			a->posti[i] = NULL;
			a->posti_occupati--;

			// logica automatica: subentro immediato del primo in lista d'attesa
			if (!coda_vuota(a->lista_attesa)){
				Studente prossimo = coda_estrai(a->lista_attesa);
				a->posti[i] = prossimo;
				a->posti_occupati++;
			}
			return 1;
		}
	}

	// studente non trovato tra i seduti
	return 0;
}

void aula_stampa_stato(Aula a){
	if (a == NULL) return;

	printf("\n--- STATO AULA: %s ---\n", a->nome);
	printf("Occupazione: %d/%d\n", a->posti_occupati, a->capienza_massima);

	printf("SEDUTI:\n");
	for (int i = 0; i < a->capienza_massima; i++){
		if (a->posti[i] != NULL){
			printf(" [%d] ", i+1);
			studente_stampa(a->posti[i]);
		}
	}

	if (coda_vuota(a->lista_attesa)){
		printf("LISTA D'ATTESA: Vuota\n");
	} else {
		printf("LISTA D'ATTESA: Ci sono persone in attesa.\n");
	}
	printf("----------------------------\n");
}
