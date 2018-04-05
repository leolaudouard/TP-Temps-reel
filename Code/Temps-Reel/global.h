/* 
 * File:   global.h
 * Author: INSA Toulouse
 *
 * Created on 20 May 2017
 */

#ifndef GLOBAL_H
#define	GLOBAL_H

#include "includes.h"
#include "lib/lib.h"

/* Defines *********************************/
#define STOP 99
#define k1 -25.8598f		/* Coefficients de l'asservissement */
#define k2 -6.5593f
#define PI  3.142f
#define TEN_SECONDS    (10*1e9)
#define SECENTOP       1200000000
#define MSG_LENGTH 50
#define MSG_QUEUE_SIZE 20
 
/* Priorités *******************************/
#define PRIORITY_TH_COMMUNICATION 80
#define PRIORITY_TH_ASSERVISSEMENT 81
//#define PRIORITY_TH_AFFICHAGE 70
#define PRIORITY_TH_AFFICHAGE 85 //Pour pouvoir afficher la perte de communication
#define PRIORITY_TH_BATTERIE 73
#define PRIORITY_TH_PRESENCEUSER 75
#define PRIORITY_TH_ENVOYER 90
#define PRIORITY_TH_ARRET_URGENCE 95

/* Variables partagées *********************/
extern int arret;
extern int etat_com;
extern int etat_reception;
extern int presence_user;


extern Angles etat_angle;
extern Battery batterie;
extern Consigne consigne_couple;
extern Beta beta ;


/* MUTEX ***********************************/
extern RT_MUTEX var_mutex_beta; 
extern RT_MUTEX var_mutex_etat_com; 
extern RT_MUTEX var_mutex_etat_angle;
extern RT_MUTEX var_mutex_consigne_couple;
extern RT_MUTEX var_mutex_batterie;
extern RT_MUTEX var_mutex_presence_user ;
extern RT_MUTEX var_mutex_etat_reception;
extern RT_MUTEX var_mutex_arret ;
extern RT_MUTEX var_mutex_watchdog ;

/* SEMAPHORES ******************************/
extern RT_SEM var_sem_arret;
extern RT_SEM var_sem_envoyer;

/* Files de messages ************************/
extern RT_QUEUE queue_Msg2STM;					// File de messages destinés au STM32

/* Tâches **********************************/
extern RT_TASK th_Communication;
extern RT_TASK th_Asservissement;
extern RT_TASK th_Affichage; 
extern RT_TASK th_Surveillance_Batterie; 
extern RT_TASK th_Envoyer; 
extern RT_TASK th_Presence_User; 
extern RT_TASK th_Arret_Urgence; 

#endif	/* GLOBAL_H */
