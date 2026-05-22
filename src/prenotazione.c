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
#include <ctype.h>
#include "prenotazione.h"

// Struttura dati nascosta per mantenere incapsulati i dettagli di data, ora e posto.
struct prenotazione {
    Studente studente;
    char data[15]; // Si aspetta del tipo GG/MM/AAAA.
    char fascia_oraria[30]; // Si aspetta del tipo 09:00-11:00.
    int stato;
    int posto_scelto;
};

/**
 * Alloca e popola i campi della nuova prenotazione, includendo il posto specifico.
 *
 * Preserva l'integrità dei dati effettuando copie sicure delle stringhe fornite.
 *
 * Parametri:
 * s: puntatore all'oggetto Studente titolare della prenotazione
 * data: stringa contenente la data richiesta
 * fascia: stringa contenente la fascia oraria richiesta
 * posto_scelto: identificativo numerico della sedia selezionata
 *
 * Ritorna:
 * Il puntatore all'istanza di Prenotazione creata, oppure NULL in caso di errore.
 */
Prenotazione prenotazione_crea(Studente s, char* data, char* fascia, int posto_scelto) {
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
    p->posto_scelto = posto_scelto;

    return p;
}

/**
 * Libera la memoria del contenitore Prenotazione azzerando il puntatore.
 *
 * Impedisce memory leak alla chiusura del programma o in fase di annullamento.
 * Non dealloca l'oggetto studente in quanto la sua gestione è indipendente.
 *
 * Parametri:
 * p: doppio puntatore all'oggetto Prenotazione da deallocare
 *
 * Ritorna:
 * Nessuno (void).
 */
void prenotazione_distruggi(Prenotazione* p) {
    if (p != NULL && *p != NULL) {
        free(*p);
        *p = NULL;
    }
}

/**
 * Fornisce l'accesso in sola lettura al riferimento dello studente.
 *
 * Garantisce l'information hiding permettendo l'interazione esterna con i dati anagrafici.
 *
 * Parametri:
 * p: puntatore all'oggetto Prenotazione da ispezionare
 *
 * Ritorna:
 * Il puntatore all'oggetto Studente, oppure NULL se la pratica è non valida.
 */
Studente prenotazione_ottieni_studente(Prenotazione p) {
    if (p == NULL) return NULL;
    return p->studente;
}

/**
 * Esegue il passaggio di stato per validare l'ingresso fisico in aula.
 *
 * Intercetta il momento esatto in cui la prenotazione diviene un accesso reale.
 *
 * Parametri:
 * p: puntatore all'oggetto Prenotazione da aggiornare
 *
 * Ritorna:
 * Nessuno (void).
 */
void prenotazione_effettua_checkin(Prenotazione p) {
    if (p != NULL) {
        p->stato = 1;
    }
}

/**
 * Restituisce l'indicatore di stato attuale della pratica.
 *
 * Utile ai report statistici per dividere accessi effettivi da assenze.
 *
 * Parametri:
 * p: puntatore all'oggetto Prenotazione da ispezionare
 *
 * Ritorna:
 * Intero che rappresenta lo stato (0 pendente, 1 validato), oppure -1 in caso di errore.
 */
int prenotazione_ottieni_stato(Prenotazione p) {
    if (p == NULL) return -1;
    return p->stato;
}

/**
 * Estrae il numero del posto a sedere riservato al momento della prenotazione.
 *
 * Consente al modulo dell'aula di conoscere l'esatta sedia da assegnare al check-in.
 *
 * Parametri:
 * p: puntatore all'oggetto Prenotazione da ispezionare
 *
 * Ritorna:
 * L'intero che rappresenta l'indice del posto, oppure -1 se l'oggetto è nullo.
 */
int prenotazione_ottieni_posto(Prenotazione p) {
    if (p == NULL) return -1;
    return p->posto_scelto;
}

/**
 * Restituisce il puntatore alla stringa contenente la data della prenotazione.
 *
 * Utilizzata per effettuare controlli incrociati sulle sovrapposizioni temporali.
 *
 * Parametri:
 * p: puntatore all'oggetto Prenotazione da ispezionare
 *
 * Ritorna:
 * La stringa della data, oppure NULL se la prenotazione non è valida.
 */
char* prenotazione_ottieni_data(Prenotazione p) {
    if (p == NULL) return NULL;
    return p->data;
}

/**
 * Restituisce il puntatore alla stringa contenente la fascia oraria.
 *
 * Necessaria per verificare la disponibilità dei posti all'interno dello stesso giorno.
 *
 * Parametri:
 * p: puntatore all'oggetto Prenotazione da ispezionare
 *
 * Ritorna:
 * La stringa della fascia oraria, oppure NULL se non valida.
 */
char* prenotazione_ottieni_fascia(Prenotazione p) {
    if (p == NULL) return NULL;
    return p->fascia_oraria;
}

/**
 * Output formattato a video dei dati essenziali della prenotazione.
 *
 * Fornisce dettagli a terminale per l'utente,
 * segnalando lo stato e il posto riservato.
 *
 * Parametri:
 * p: puntatore all'oggetto Prenotazione da mostrare
 *
 * Ritorna:
 * Nessuno (void).
 */
void prenotazione_stampa(Prenotazione p) {
    if (p != NULL) {
        printf("Data: %s | Fascia: %s | Posto: %02d | Stato: %s\n",
               p->data,
               p->fascia_oraria,
               p->posto_scelto,
               (p->stato == 1) ? "CHECK-IN EFFETTUATO" : "IN ATTESA");
        printf("  -> ");
        studente_stampa(p->studente);
    }
}

/**
 * Verifica che la stringa fornita rispetti il formato di una data espressa come "GG/MM/AAAA".
 *
 * Utilizzata per assicurare che gli input dell'utente siano strutturalmente e logicamente validi.
 *
 * Parametri:
 * data: stringa costante contenente la data da analizzare
 *
 * Ritorna:
 * 1 se la stringa rispetta i vincoli di formato e coerenza del calendario, 0 altrimenti.
 */
int valida_data(const char* data){
	if(strlen(data) != 10){
			return 0;
			}
	if(data[2] != '/' || data[5] != '/'){
			 return 0;
			}

	for(int i = 0; i < 10; i++){
		// Non considero i casi dei separatori
		if(i == 2 || i == 5) continue;
		// Se un carattere non è numerico (da 0 a 9), la data non è valida
		if(!isdigit(data[i])){
				return 0;
				}
			}

		// Controllo se la data è accettabile
		// Separo la data in giorno, mese e anno
		char stringa_giorno[3] = {data[0], data[1], '\0'};
		char stringa_mese[3] = {data[3], data[4], '\0'};
		char stringa_anno[5] = {data[6], data[7], data[8], data[9], '\0'};

		// Converto le stringhe in interi
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

		// Creo un array che contiene i giorni massimi per ogni mese
		int giorni_massimi[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

		// Gestione anno bisestile per febbraio
		if((anno % 4 == 0 && anno % 100 != 0) || (anno % 400 == 0)){
					   giorni_massimi[1] = 29;
					}

		if(giorno > giorni_massimi[mese-1]){
					return 0;
					}

	return 1;
}

/**
 * Verifica che la stringa fornita rispetti il formato di una fascia oraria "HH:MM-HH:MM".
 *
 * Implementa i vincoli temporali per garantire che l'ora di uscita sia successiva all'ingresso.
 *
 * Parametri:
 * fascia: stringa costante contenente l'orario da analizzare
 *
 * Ritorna:
 * 1 se la struttura, i caratteri e la logica temporale sono coerenti, 0 altrimenti.
 */
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
		// Non considero i casi dei separatori
		if(i == 2 || i == 5 || i == 8) continue;
		// Se un carattere non è numerico (da 0 a 9), la fascia oraria non è valida
		if(!isdigit(fascia[i])){
				return 0;
				}
			}

		// Controllo se l'orario è accettabile
		// Separo ore e minuti
		char str_ora_in[3] = {fascia[0], fascia[1], '\0'};
		char str_min_in[3] = {fascia[3], fascia[4], '\0'};
		char str_ora_out[3] = {fascia[6], fascia[7], '\0'};
		char str_min_out[3] = {fascia[9], fascia[10], '\0'};

		// Converto in interi
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

		if(min_out < 0 || min_out > 59){
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
