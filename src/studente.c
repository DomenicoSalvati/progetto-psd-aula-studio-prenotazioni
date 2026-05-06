#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "studente.h"

struct studente {
	char matricola[20];
	char nome[100];
	char corso[100];

};


 Studente studente_crea(char* matricola, char* nome, char* corso){
	 if (matricola == NULL || nome == NULL || corso == NULL){
		 return NULL;
				}


 Studente s = (Studente) malloc(sizeof(struct studente)); //eseguo sempre il cast al tipo
	 if (s == NULL){
 	 return NULL;
		}

 strncpy(s->matricola, matricola, sizeof(s->matricola)-1);
 s->matricola[sizeof(s->matricola)-1] = '\0';

 strncpy(s->nome, nome, sizeof(s->nome)-1);
 s->nome[sizeof(s->nome)-1] = '\0';

 strncpy(s->corso, corso, sizeof(s->corso)-1);
 s->corso[sizeof(s->corso)-1] = '\0';

 return s;
}



void studente_distruggi(Studente* s){
	if (s != NULL && *s != NULL){
		free(*s);
		*s = NULL;
		}

 }



char* studente_ottieni_matricola(Studente s){
	  if (s == NULL){
	    return NULL;
	  }
	return s->matricola;

 }



void studente_stampa(Studente s){
	if (s != NULL){
	   printf("[%s] %s - %s\n", s->matricola, s->nome, s->corso);
		}

 }

