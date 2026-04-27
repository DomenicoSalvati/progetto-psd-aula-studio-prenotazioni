#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "studente.h"
#include "aula.h"

// Definizione della struct (nel .c per garantire information hiding)

struct studente {
	char matricola[20];
	char nome[100];
	char corso[100];

};

 // Implemento le funzioni

 Studente studente_crea(char* matricola, char* nome, char* corso){
	 // controllo sui parametri in ingresso
	 if (matricola == NULL || nome == NULL || corso == NULL){
		 return NULL;
				}

 // alloco dinamicamente la memoria per la struct
 Studente s = (Studente) malloc(sizeof(struct studente));
	 if (s == NULL){
 	 return NULL; //allocazione fallita
		}

 //copio le stringhe in modo sicuro per evitare buffer overflow
 strncpy(s->matricola, matricola, sizeof(s->matricola)-1);
 s->matricola[sizeof(s->matricola)-1] = '\0'; //per garantire il terminatore

 strncpy(s->nome, nome, sizeof(s->nome)-1);
 s->nome[sizeof(s->nome)-1] = '\0';

 strncpy(s->corso, corso, sizeof(s->corso)-1);
 s->corso[sizeof(s->corso)-1] = '\0';

 return s;
}



void studente_distruggi(Studente* s){
	//controllo che il puntatore e la struct non siano già nulli
	if (s != NULL && *s != NULL){
		free(*s); //libero la memoria allocata con malloc
		*s = NULL;
		}

 }



char* studente_get_matricola(Studente s){
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
