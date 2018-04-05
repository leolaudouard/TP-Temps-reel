/* 
 * File:   global.c
 * Author: INSA Toulouse
 *
 * Created on 20 May 2017
 */

#include "global.h"

/* Déclaration des variables partagées *****/
int arret = 0;
int etat_reception = 0;
int etat_com = 0;
int presence_user = 1;

Angles etat_angle;
Battery batterie;
Consigne consigne_couple;
Beta beta ;

/* MUTEX ***********************************/
RT_MUTEX var_mutex_etat_com; 
RT_MUTEX var_mutex_beta; 
RT_MUTEX var_mutex_etat_angle;
RT_MUTEX var_mutex_consigne_couple;
RT_MUTEX var_mutex_etat_reception;
RT_MUTEX var_mutex_batterie;
RT_MUTEX var_mutex_presence_user ;
RT_MUTEX var_mutex_arret ;
RT_MUTEX var_mutex_watchdog ;

/* SEMAPHORES ******************************/
RT_SEM var_sem_arret;
RT_SEM var_sem_envoyer;

/* Files de messages ***********************/
RT_QUEUE queue_Msg2STM;

/* Tâches **********************************/
RT_TASK th_Communication;
RT_TASK th_Asservissement;
RT_TASK th_Affichage; 
RT_TASK th_Surveillance_Batterie; 
RT_TASK th_Envoyer; 
RT_TASK th_Presence_User; 
RT_TASK th_Arret_Urgence;

