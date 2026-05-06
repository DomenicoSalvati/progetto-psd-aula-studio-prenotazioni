#include <stdlib.h>
#include <string.h>
#include "aula.h"

// definizione della struct per information hiding
struct aula {
	char nome[100];
	int capienza_massima;
	int posti_occupati;
	Studente* posti; // array dinamico per i posti fisici
};

// implemento le funzioni

Aula aula_crea(char* nome, int capienza_massima){
	// controllo sui parametri in ingresso
	if (nome == NULL || capienza_massima <= 0){
		return NULL;
	}

	// alloco dinamicamente la memoria per la struct
	Aula a = (Aula) malloc(sizeof(struct aula));
	if (a == NULL){
		return NULL; // allocazione fallita
	}

	// copio la stringa in modo sicuro per evitare buffer overflow
	strncpy(a->nome, nome, sizeof(a->nome)-1);
	a->nome[sizeof(a->nome)-1] = '\0'; // per garantire il terminatore

	a->capienza_massima = capienza_massima;
	a->posti_occupati = 0;

	// uso calloc per allocare l'array e inizializzare i puntatori a NULL (posti liberi)
	a->posti = (Studente*) calloc(capienza_massima, sizeof(Studente));
	if (a->posti == NULL){
		free(a);
		return NULL;
	}

	return a;
}

void aula_distruggi(Aula* a){
	// controllo che il puntatore e la struct non siano già nulli
	if (a != NULL && *a != NULL){
		if ((*a)->posti != NULL){
			free((*a)->posti); // libero prima l'array dei posti
		}
		free(*a); // libero la memoria allocata con malloc
		*a = NULL;
	}
}

// gestisce l'ingresso senza prenotazione (cerca posto libero)
int aula_ingresso_libero(Aula a, Studente s){
	// controllo sui parametri in ingresso
	if (a == NULL || s == NULL){
		return 0; // 0 = errore o parametri nulli
	}

	// controllo se ci sono posti fisicamente disponibili
	if (a->posti_occupati >= a->capienza_massima){
		return 0; // aula piena
	}

	// cerco il primo posto libero (NULL) nell'array
	for (int i = 0; i < a->capienza_massima; i++){
		if (a->posti[i] == NULL){
			a->posti[i] = s; // faccio sedere lo studente
			a->posti_occupati++;
			return 1; // 1 = successo
		}
	}

	return 0;
}

// registra l'uscita di uno studente e libera il posto
int aula_uscita(Aula a, Studente s){
	// controllo sui parametri in ingresso
	if (a == NULL || s == NULL){
		return 0;
	}

	// cerco lo studente nell'array dei posti
	for (int i = 0; i < a->capienza_massima; i++){
		if (a->posti[i] == s){
			a->posti[i] = NULL; // libero fisicamente il posto
			a->posti_occupati--;
			return 1; // 1 = uscito con successo
		}
	}

	// se arrivo qui, lo studente non era in questa aula
	return 0;
}
