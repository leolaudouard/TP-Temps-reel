/* 
 * File:   fonctions.c
 * Author: INSA Toulouse
 *
 * Created on 20 May 2017
 */

#include "fonctions.h"

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