/* 
 * File:   fonctions.c
 * Author: INSA Toulouse
 *
 * Created on 20 May 2017
 */

#include "fonctions.h"

/* La tâche Asservissement, en fonction de la mise à jour ou non des valeurs du STM32,
 * calcule puis envoie dans la file de message une nouvelle consigne de courant pour le STM
 * fonctions : rt_queue_write pour écrire dans la file de message
 * variables : etat_com, etat_angle, etat_reception, consigne_couple
 * semaphore : sem_envoyer */
void Asservissement(void *arg) 
{
	
	rt_printf("Thread Asservissement: Debut de l'exécution de periodique à 50 Hz\n");
	rt_task_set_periodic(NULL, TM_NOW, 20000000);

	log_task_entered();
	
	while (1) {

		rt_task_wait_period(NULL);
	
	}
}

/* La tâche Presence_User vérifie périodiquement la présence de l'utilisateur sur le gyropode
 *  déclenche l'arrêt d'urgence si l'utilisateur n'est pas présent
 * variables : etat_com, presence_user
 * semaphore : sem_arret */
 void Presence_User(void *arg) 
{
	
}

/* La tâche Surveillance_Batterie vérifie périodiquement le niveau de batterie du gyropode
 *  déclenche l'arrêt d'urgence si la batterie est trop faible (<15%)
 * variables : etat_com, presence_user, batterie, arret
 * semaphore : sem_arret */
 void Surveillance_Batterie(void *arg) /* OK */
{
	int com, arr, pres;
	int bat_lvl;

	rt_printf("Thread Surveillance_Batterie : Debut de l'éxecution de periodique à 1 Hz\n");
	rt_task_set_periodic(NULL, TM_NOW, 1000000000);

	log_task_entered();
	
	while (1) {
  
		rt_task_wait_period(NULL);
		
	}
}

/* La tâche Communication est chargée de mettre en place la communication avec le STM32,
 *  à la réception d'un mesage du STM, elle décripte la trame et met à jour les informations
 *  des variables partagées.
 * fonctions : init_serial, read_from_serial, printf_trame
 * variables : etat_com, etat_reception */
void Communication(void *arg) 
{
	int uart0_filestream = -1;
	int i;
	int com = 0;
	int etat_rep=0;
	unsigned char tx_buffer[20];
	unsigned char *p_tx_buffer;
	message_serial *m;
	
	rt_printf("Thread Communication : Debut de l'éxecution de periodique à 50 Hz\n");
	rt_task_set_periodic(NULL, TM_NOW, 20000000);

	log_task_entered();

	while (1) {

			rt_task_wait_period(NULL);

			uart0_filestream = init_serial();// Initialise la connexion avec l'UART

        	if(uart0_filestream == -1){      
				rt_printf("Can't Use the UART\n"); 
			}
			else{        
		       	m = (message_serial*)malloc(5*sizeof(message_serial));
		       	m = read_from_serial();
				write_trame_to_data(m);		
			}
	 }   
}

/*  La tâche Affichage communique à l'interface graphique (GUI) (qui s'éxécute dans le noyau Linux) à travers un socket, 
 *  les valeurs des variables partagées du programme de temps réel
 *  cette fonction n'a pas à être modifiée, à part modification du GUI et ajout d'informations à envoyer 
 * fonctions : add_info_float, send_trame
 * variables : etat_angle, vitesse_lin, consigne_couple, batterie, presence_user, etat_com, arret */
void Affichage(void *arg){

	unsigned char *str;
	int indice=0; 
	int	bat,user,stop,com;
	float tamp1=0.0f,tamp2=0.0f,tamp3=0.0f,tamp4=0.0f;
	int sckt_gui;

	sckt_gui = init_socket(PORT_GUI);

	rt_printf("Thread Affichage : Debut de l'éxecution de periodique à 10 Hz\n");
	rt_task_set_periodic(NULL, TM_NOW, 100000000);

	log_task_entered();
	
	while(1){

		rt_task_wait_period(NULL);

		str = (unsigned char*)malloc(56* sizeof(unsigned char)); // 56 = (n * 7 ) avec n nombre de fonctions "add_info"

		rt_mutex_acquire(&var_mutex_etat_angle, TM_INFINITE);
		log_mutex_acquired(&var_mutex_etat_angle);

		tamp1 = etat_angle.angle();
		tamp2 = etat_angle.vitesse_ang();

		rt_mutex_release(&var_mutex_etat_angle);
		log_mutex_released(&var_mutex_etat_angle);

		rt_mutex_acquire(&var_mutex_beta, TM_INFINITE);
		log_mutex_acquired(&var_mutex_beta);

		tamp3 = beta.beta();

		rt_mutex_release(&var_mutex_beta);
		log_mutex_released(&var_mutex_beta);

		rt_mutex_acquire(&var_mutex_consigne_couple, TM_INFINITE);
		log_mutex_acquired(&var_mutex_consigne_couple);

		tamp4=consigne_couple.consigne();

		rt_mutex_release(&var_mutex_consigne_couple);
		log_mutex_released(&var_mutex_consigne_couple);

		rt_mutex_acquire(&var_mutex_batterie, TM_INFINITE);
		log_mutex_acquired(&var_mutex_batterie);

		bat = batterie.level();

		rt_mutex_release(&var_mutex_batterie);
		log_mutex_released(&var_mutex_batterie);
			
		rt_mutex_acquire(&var_mutex_presence_user, TM_INFINITE);
		log_mutex_acquired(&var_mutex_presence_user);

		user=presence_user;

		rt_mutex_release(&var_mutex_presence_user);
		log_mutex_released(&var_mutex_presence_user);

		rt_mutex_acquire(&var_mutex_etat_com, TM_INFINITE);
		log_mutex_acquired(&var_mutex_etat_com);

	  	com = etat_com;

		rt_mutex_release(&var_mutex_etat_com);
		log_mutex_released(&var_mutex_etat_com);

		rt_mutex_acquire(&var_mutex_arret, TM_INFINITE);
		log_mutex_acquired(&var_mutex_arret);

		stop = arret;

		rt_mutex_release(&var_mutex_arret);
		log_mutex_released(&var_mutex_arret);

 		add_info_float(str,'p',tamp1,&indice);
		add_info_float(str,'s',tamp2,&indice);
		add_info_float(str,'v',tamp3,&indice);
		add_info_float(str,'c',tamp4,&indice);

 		add_info_float(str,'b',bat,&indice);
		add_info_float(str,'u',user,&indice);
		add_info_float(str,'e',com,&indice);
		add_info_float(str,'a',stop,&indice);

		send_trame(sckt_gui, str, &indice);
    	}
}

/* Cette fonction peut être déclenchée par les threads Presence_user et Surveillance_Batterie,
 *  elle envoie alors sur la file de messages un message de type arrêt ('a',1) qui sera envoyé au STM32
 * fonctions : rt_queue_write pour écrire dans la file de message, rt_sem_p pour attendre la libération du sémaphore
 * variables : arret */

void Arret_Urgence(void *arg){	

	log_task_entered();

	while(1){
		rt_task_wait_period(NULL);
	}
}

/* La tâche Envoyer est simplement chargée d'envoyer les messages contenus dans la file de messages
 *  au STM32 à travers la liaison UART
 * fonctions : rt_queue_read, send_float_to serial, send_int_to_serial
 * variables : etat_com, etat_angle, etat_reception, consigne_couple */

void Envoyer(void *arg){

	rt_printf("Thread Envoyer : Debut de l'éxecution de periodique à 100 Hz\n");
	rt_task_set_periodic(NULL, TM_NOW, 10000000);

	log_task_entered();

	while(1){
		rt_task_wait_period(NULL);

		message_stm m;
		int err = rt_queue_read(&queue_Msg2STM,&m,sizeof(message_stm),SECENTOP / 10000);

		if(m.label == 'c'){
			send_float_to_serial(m.fval,'c');
		}
		else if(m.label == 'a'){
			send_int_to_serial(m.ival,'a');
		}
	}
}
