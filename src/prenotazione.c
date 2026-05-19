/**
 * File: prenotazione.c
 * Autore: Domenico
 * Data: Maggio 2026
 *
 * Descrizione: Implementazione concreta dell'ADT Prenotazione.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "prenotazione.h"
#include <ctype.h>

// Struttura dati nascosta per mantenere incapsulati i dettagli di data e ora.
struct prenotazione {
    Studente studente;
    char data[15]; // Si aspetta del tipo GG/MM/AAAA.
    char fascia_oraria[30]; // Si aspetta del tipo 09:00-11:00.
    int stato;
};

// Alloca e popola i campi della nuova prenotazione.
Prenotazione prenotazione_crea(Studente s, char* data, char* fascia) {
    if (s == NULL || data == NULL || fascia == NULL) {
        return NULL;
    }

    Prenotazione p = (Prenotazione) malloc(sizeof(struct prenotazione));
    if (p == NULL) return NULL;

    p->studente = s;

    strncpy(p->data, data, sizeof(p->data) - 1);
    p->data[sizeof(p->data) - 1] = '\0';

    strncpy(p->fascia_oraria, fascia, sizeof(p->fascia_oraria) - 1);
    p->fascia_oraria[sizeof(p->fascia_oraria) - 1] = '\0';

    p->stato = 0;

    return p;
}

// Libera la memoria del contenitore Prenotazione.
void prenotazione_distruggi(Prenotazione* p) {
    if (p != NULL && *p != NULL) {
        free(*p);
        *p = NULL;
    }
}

// Accesso in sola lettura al riferimento dello studente.
Studente prenotazione_ottieni_studente(Prenotazione p) {
    if (p == NULL) return NULL;
    return p->studente;
}

// Esegue il passaggio di stato per validare l'ingresso fisico in aula.
void prenotazione_effettua_checkin(Prenotazione p) {
    if (p != NULL) {
        p->stato = 1;
    }
}

// Restituisce l'indicatore di stato attuale della pratica.
int prenotazione_ottieni_stato(Prenotazione p) {
    if (p == NULL) return -1;
    return p->stato;
}

// Output formattato a video dei dati essenziali della prenotazione.
void prenotazione_stampa(Prenotazione p) {
    if (p != NULL) {
        printf("Data: %s | Fascia: %s | Stato: %s\n",
               p->data,
               p->fascia_oraria,
               (p->stato == 1) ? "CHECK-IN EFFETTUATO" : "IN ATTESA");
        printf("  -> ");
        studente_stampa(p->studente);
    }
}

int valida_data(const char* data){
	if(strlen(data) != 10){
			return 0;
			}
	if(data[2] != '/' || data[5] != '/'){
			 return 0;
			}


	for(int i = 0; i < 10; i++){
		//Non considero i casi precedenti
		if(i == 2 || i == 5) continue;
		//Se un caratterenon è numerico(da 0 a 9), la data non è valida
		if(!isdigit(data[1])){
				return 0;
				}

			}


		//Controllo se la data è accettabile
		//Separo la data in giorno, mese e anno
		char stringa_giorno[3] = {data[0], data[1], '\0'};
		char stringa_mese[3] = {data[3], data[4], '\0'};
		char stringa_anno[5] = {data[6], data[7], data[8], data[9], '\0'};

		//Converto le stringhe in interi
		int giorno = atoi(stringa_giorno);
		int mese = atoi(stringa_mese);
		int anno = atoi (stringa_anno);

		if(mese < 1 || mese > 12){
				return 0;
				}

		if(anno < 2026 || anno > 2030){
				return 0;
				}


		if(giorno < 1){
				return 0;
				}


		//Creo un array che contiene i giorni massimi per ogni mese
		int giorni_massimi[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

		//Gestione anno bisestile per febbraio
		if((anno % 4 == 0 && anno % 100 != 0) || (anno % 400 == 0)){
					   giorni_massimi[1] = 29;
					}

		if(giorno > giorni_massimi[mese-1]){
					return 0;
					}

	return 1;

}


int valida_fascia(const char* fascia){
	if(strlen(fascia) != 11){
			return 0;
			}
	if(fascia[2] != ':' || fascia[8] != ':'){
			return 0;
			}
	if(fascia[5] != '-'){
			return 0;
			}

	for(int i = 0; i < 11; i++){
		//Non considero i casi precedenti
		if(i == 2 || i == 5 || i == 8) continue;
		//Se un carattere non è numerico(da 0 a 9), la fascia oraria non è valida
		if(!isdigit(fascia[i])){
				return 0;
				}

			}


		//Controllo se l'orario è accettabile
		//Separo ore e minuti
		char str_ora_in[3] = {fascia[0], fascia[1], '\0'};
		char str_min_in[3] = {fascia[3], fascia[4], '\0'};
		char str_ora_out[3] = {fascia[6], fascia[7], '\0'};
		char str_min_out[3] = {fascia[9], fascia[10], '\0'};


		//Converto in interi
		int ora_in = atoi(str_ora_in);
		int min_in = atoi(str_min_in);
		int ora_out = atoi(str_ora_out);
		int min_out = atoi(str_min_out);


		if(ora_in < 0 || ora_in > 23){
				return 0;
				}

		if(ora_out < 0 || ora_out > 23){
				return 0;
				}

		if(min_in < 0 || min_in > 59){
				return 0;
				}

		if(min_out < 0 || min_out >59){
				return 0;
				}

		// Vincolo logico temporale: l'orario di uscita deve essere dopo l'orario di ingresso
		int tempo_ingresso_minuti = (ora_in*60) + min_in;
		int tempo_uscita_minuti = (ora_out*60) + min_out;

		if(tempo_uscita_minuti <= tempo_ingresso_minuti){
					return 0;
					}

		return 1;


}
