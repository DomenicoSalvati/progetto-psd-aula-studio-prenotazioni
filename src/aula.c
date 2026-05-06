#include <stdlib.h>
#include <string.h>
#include "aula.h"

struct aula {
	char nome[100];
	int capienza_massima;
	int posti_occupati;
	Studente* posti; // array dinamico per i posti fisici
};


Aula aula_crea(char* nome, int capienza_massima){
	if (nome == NULL || capienza_massima <= 0){
		return NULL;
	}

	Aula a = (Aula) malloc(sizeof(struct aula));
	if (a == NULL){
		return NULL;
	}

	strncpy(a->nome, nome, sizeof(a->nome)-1);
	a->nome[sizeof(a->nome)-1] = '\0';

	a->capienza_massima = capienza_massima;
	a->posti_occupati = 0;

	// uso calloc (con cast) per inizializzare i puntatori a NULL (direttamente posti liberi)
	a->posti = (Studente*) calloc(capienza_massima, sizeof(Studente));
	if (a->posti == NULL){
		free(a);
		return NULL;
	}

	return a;
}

void aula_distruggi(Aula* a){
	if (a != NULL && *a != NULL){
		if ((*a)->posti != NULL){
			free((*a)->posti); //libero array
		}
		free(*a);
		*a = NULL;
	}
}

int aula_ingresso_libero(Aula a, Studente s){
	if (a == NULL || s == NULL){
		return 0;
	}

	if (a->posti_occupati >= a->capienza_massima){
		return 0;
	}


	for (int i = 0; i < a->capienza_massima; i++){ //cerco il posto libero
		if (a->posti[i] == NULL){
			a->posti[i] = s;
			a->posti_occupati++;
			return 1;
		}
	}

	return 0;
}

int aula_uscita(Aula a, Studente s){
	if (a == NULL || s == NULL){
		return 0;
	}

	// cerco lo studente nell'array dei posti
	for (int i = 0; i < a->capienza_massima; i++){
		if (a->posti[i] == s){
			a->posti[i] = NULL;
			a->posti_occupati--;
			return 1;
		}
	}

	return 0;
}
