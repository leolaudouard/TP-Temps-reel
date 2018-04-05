/* 
 * File:  fonctions.h
 * Author: INSA Toulouse
 *
 * Created on 20 May 2017
 */

#ifndef FONCTIONS_H
#define	FONCTIONS_H

#include "includes.h"

#ifdef	__cplusplus
extern "C" {
#endif

void Envoyer(void *arg);
void Asservissement(void * arg);
void Presence_User(void * arg);
void Surveillance_Batterie(void * arg);
void Communication(void * arg);
void Affichage(void * arg);
void Arret_Urgence(void *arg);

#ifdef	__cplusplus
}
#endif

#endif	/* FONCTIONS_H */

