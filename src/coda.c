#include <stdio.h>
#include <stdlib.h>
#include "item.h"
#include "coda.h"

struct nodo {
	item valore;
	struct nodo *prossimo;
};

struct c_coda {
	struct nodo *testa, *fondo;
	int n_elementi;
};

Coda coda_crea(void){
	struct c_coda *q = malloc(sizeof(struct c_coda));
	if (q == NULL) return NULL;

	q->n_elementi = 0;
	q->testa = NULL;
	q->fondo = NULL;
	return q;
} 

int coda_vuota(Coda q){
	if (q == NULL) return -1;
	return q->n_elementi == 0;
}

int coda_inserisci(item val, Coda q){
	if (q == NULL) return -1;

	struct nodo *nuovo = malloc(sizeof(struct nodo));
	if (nuovo == NULL) return 0;

	nuovo->valore = val;
	nuovo->prossimo = NULL;

	if (q->testa == NULL) {
		q->testa = nuovo;
	} else {
		q->fondo->prossimo = nuovo;
	}

	q->fondo = nuovo;
	(q->n_elementi)++;
	return 1;
}

item coda_estrai(Coda q){
	if (q == NULL || q->n_elementi == 0) {
		return NULLITEM;
	}

	item risultato = q->testa->valore;
	struct nodo *temp = q->testa;

	q->testa = q->testa->prossimo;
	free(temp);

	if (q->testa == NULL) {
		q->fondo = NULL;
	}

	(q->n_elementi)--;
	return risultato;
}

void coda_distruggi(Coda* q){
	if (q != NULL && *q != NULL){
		while (!coda_vuota(*q)){
			coda_estrai(*q); // estraggo e libero ogni singolo nodo
		}
		free(*q);
		*q = NULL;
	}
}
