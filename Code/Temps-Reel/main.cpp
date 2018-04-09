/**
 * \file main.c
 * \brief Source de la classe Angles
 * \author INSA Toulouse
 * \version 1.0
 * \date 15 Septembre 17
 *
 * Programme principale temps réel
 *
 */

//Include
#include "includes.h"
#include <iostream>

//Declaration des fonctions
void initStruct(void);
void startTasks(void);
void deleteTasks(void);

/**
     * \fn Programme principle de temps réel
     */
int main(void){

	/* disable memory swap */
	mlockall(MCL_CURRENT | MCL_FUTURE);	
	init_recording();		// Initialisation de l'enregistrement des évènements par le log
	initStruct();				// Création des tâches, mutex, sémaphores, files de messages
	startTasks();			// Démarre toutes les tâches
	pause();
	deleteTasks();
	stop_recording();
	return 0;
}

/**
     * \fn initStruct
     * \brief Initialisation des mutex, semaphores , des tâches et file de message
     */
void initStruct(void) {
	int err;

	/**************************************************************************************/
	/* 																Creation des mutex 												  */
	/**************************************************************************************/
	if(err = rt_mutex_create(&var_mutex_etat_com, "mutex_etat_com")){
		rt_printf("Error mutex create: %s\n", strerror(-err));
		exit(EXIT_FAILURE);
	}

	if(err = rt_mutex_create(&var_mutex_beta, "mutex_beta")){
		rt_printf("Error mutex create: %s\n", strerror(-err));
		exit(EXIT_FAILURE);
	}

	if(err = rt_mutex_create(&var_mutex_etat_angle, "mutex_etat_angle")){
		rt_printf("Error mutex create: %s\n", strerror(-err));
		exit(EXIT_FAILURE);
	}

	if(err = rt_mutex_create(&var_mutex_consigne_couple, "mutex_consigne_couple")){
		rt_printf("Error mutex create: %s\n", strerror(-err));
		exit(EXIT_FAILURE);
	}

	if(err = rt_mutex_create(&var_mutex_batterie, "mutex_batterie")){
		rt_printf("Error mutex create: %s\n", strerror(-err));
		exit(EXIT_FAILURE);
	}

	if(err = rt_mutex_create(&var_mutex_etat_reception, "mutex_reception")){
		rt_printf("Error mutex create: %s\n", strerror(-err));
		exit(EXIT_FAILURE);
	}

	if(err = rt_mutex_create(&var_mutex_watchdog, "mutex_watchdog")){
		rt_printf("Error mutex create: %s\n", strerror(-err));
		exit(EXIT_FAILURE);
	}

	if(err = rt_mutex_create(&var_mutex_presence_user, "mutex_presence_utilisateur")){
		rt_printf("Error mutex create: %s\n", strerror(-err));
		exit(EXIT_FAILURE);
	}

	if(err = rt_mutex_create(&var_mutex_arret, "mutex_arret")){
		rt_printf("Error mutex create: %s\n", strerror(-err));
		exit(EXIT_FAILURE);
	}

	rt_printf("Fin init mutex\n");


	/**************************************************************************************/
	/* 													Creation des semaphores 												  */
	/**************************************************************************************/

	if(err = rt_sem_create(&var_sem_arret, "var_sem_arret",0,TM_INFINITE)){
		rt_printf("Error semaphore create: %s\n", strerror(-err));
		exit(EXIT_FAILURE);
	}

	if(err = rt_sem_create(&var_sem_envoyer, "var_sem_envoyer",0,TM_INFINITE)){
		rt_printf("Error semaphore create: %s\n", strerror(-err));
		exit(EXIT_FAILURE);
	}

	rt_printf("Fin init semaphores\n");


	/**************************************************************************************/
	/* 																Creation des taches 												  */
	/**************************************************************************************/

	if (err = rt_task_create(&th_Communication, "Communication", 0, PRIORITY_TH_COMMUNICATION, 0)){
		rt_printf("Error task create: %s\n", strerror(-err));
		exit(EXIT_FAILURE);
	}

	if (err = rt_task_create(&th_Asservissement, "Asservissement", 0, PRIORITY_TH_ASSERVISSEMENT, 0)){
		rt_printf("Error task create: %s\n", strerror(-err));
		exit(EXIT_FAILURE);
	}

	if (err = rt_task_create(&th_Affichage, "Affichage", 0, PRIORITY_TH_AFFICHAGE, 0)){
		rt_printf("Error task create: %s\n", strerror(-err));
		exit(EXIT_FAILURE);
	}

	if (err = rt_task_create(&th_Envoyer, "Envoyer", 0, PRIORITY_TH_ENVOYER, 0)){
		rt_printf("Error task create: %s\n", strerror(-err));
		exit(EXIT_FAILURE);
	}

    rt_printf("Fin init tache\n");


	/**************************************************************************************/
	/* 												Creation la file de message 												  */
	/**************************************************************************************/

	int err1 = 0;
	err1 = rt_queue_create(&queue_Msg2STM, "Queue_stm", sizeof(message_stm)*50, Q_UNLIMITED, Q_FIFO);
	if (err1<0){
		switch(err){
			case (-EINVAL):
				rt_printf("rt_queue_create error: invalid mode or poolsize is 0\n");
				break;
			case (-ENOMEM):
				rt_printf("rt_queue_create error: system fails to get memory \n");
				break;
			case (-EEXIST):
				rt_printf("rt_queue_create error: name conflict \n");
				break;
			case (-EPERM):
				rt_printf("rt_queue_create error: called by asynchronous context \n");
				break;
		}
	}
		else rt_printf("Queue créée avec succès\n");
}


/**
     * \fn startTasks
     * \brief Lancer tous les tâches crées
     */
void startTasks() {
	int err;

	if (err = rt_task_start(&th_Communication, &Communication, NULL)){
		rt_printf("Error task start: %s\n", strerror(-err));
		exit(EXIT_FAILURE);
	}

	if (err = rt_task_start(&th_Affichage, &Affichage, NULL)){
		rt_printf("Error task start: %s\n", strerror(-err));
		exit(EXIT_FAILURE);
	}


}


/**
     * \fn deleteTasks
     * \brief Supprimer tous les tâches crées
     */
void deleteTasks() {

	rt_task_delete(&th_Communication);
	rt_task_delete(&th_Asservissement);
	rt_task_delete(&th_Affichage);
	rt_task_delete(&th_Envoyer);

}


