#include "gui.h"
#include "global.h"
#include <stdio.h>


/**
     * \fn Fonction initialise un socket, et attend la connexion du client
     * \brief Mise à jour des données décrit par un objet Angles.
     * \param port numéro du port
     * \return Identifiant du socket créé 
     */
int init_socket(int port){

	int sockfd, newsockfd;
	socklen_t clilen;
	struct sockaddr_in cli_addr, serv_addr;

	/* ouvre un socket UDP */
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("can't open stream socket");
	}
	/* bind de l'adresse locale */
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(port);
	if(bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
		perror("can't bind local address");
	}
	/* écoute du socket */
	listen(sockfd, 5);
	
	/* attente d'une connexion par le client */
 	clilen = sizeof(cli_addr);
 	newsockfd = accept(sockfd, &cli_addr, &clilen);

 	if(newsockfd < 0) {
    	perror("can't bind local address");
 	}
	return newsockfd;
}

/**
     * \fn read_socket_values()
     * \brief Lit un message sur le socket, l'analyse et met à jour les variables globales contenant les valeurs numériques à afficher 
*/
void read_socket_values(){
	int len,i,taille;
	unsigned char tab[MAX_SIZE];
	unsigned char string[MAX_SIZE];
	float data;

	/* lecture d'un message de taille max MAX_SIZE, information dans string */
 	len = read(sckt_gui, string, MAX_SIZE); 
	taille=strlen((char*)string);

	if(taille > 0){
		memcpy(tab,string,taille);	//Si le message n'est pas vide, on copie ces informations dans tab

		for(i=0 ; i<len ; i++){
				if((tab[i] == '<')&&(tab[i+6] == '\n')){
					switch (tab[i+1]){
						//couple
						case 'c':		
								memcpy(&data,&tab[i+2],sizeof(data));
								couple = data -100000.05f ; // retrait de l'offset ajouté par la RPI lors de l'envoi
								break;	
						//angle beta
						case 'v' : 
								//dans la maquette réel, il s'agit de vitesse linéaire
								memcpy(&data,&tab[i+2],sizeof(data));
								vit_lin = (data -100000.05f)*180.0f/3.142f ; //conversion en degrés
								break;
						//battery
						case 'b' : 
								memcpy(&data,&tab[i+2],sizeof(data));							
								bat_lvl = data -100000.05f ;
								break;
						//vitesse angulaire
						case 's' :
								memcpy(&data,&tab[i+2],sizeof(data));
								vit_ang = (data -100000.05f)*180.0f/3.142f; //conversion en degrés
								break;
						//angle
						case 'p' : 
								memcpy(&data,&tab[i+2],sizeof(data));
								angle = (data -100000.05f)*180.0f/3.142f; //conversion en degrés
								break;
						//etat communication
						case 'e' : 
								memcpy(&data,&tab[i+2],sizeof(data));	
								etat_com = data -100000.05f  ;
								break;	
						//presence user
						case 'u' : 
								memcpy(&data,&tab[i+2],sizeof(data));	
								presence =data -100000.05f ;
								break;	
						//arret urgence
						case 'a' : 
								memcpy(&data,&tab[i+2],sizeof(data));	
								arret = data -100000.05f  ;
								break;	
					}//case
				}//<\n
		} //for
	} //if taille
}

/**
     * \fn gboolean update_info(GtkWidget *widget)
     * \brief Actualise les infos communiquées a l'utilisateur (text_view appelé info)
	 * \param GTKWidget
*/
gboolean update_info(GtkWidget *widget){
	char *str;
	int line = FALSE;
	str = malloc(256);	

	if (widget == NULL) return FALSE;
	
	if (bat_lvl < 15){
		str = strcpy(str,"ATTENTION !!!! Niveau de batterie critique !");
		g_utf8_make_valid(str,strlen(str));
		line = TRUE;
	}
	else if (bat_lvl < 30){
		str = strcpy(str,"Batterie faible !");
		g_utf8_make_valid(str,strlen(str));
		line = TRUE;
	}

	 if (presence==0){
		str = strcpy(str,"Absence de l'utilisateur");
		g_utf8_make_valid(str,strlen(str));
		line = TRUE;
	}
	if (etat_com == 0 && line == FALSE){
		str = strcpy(str,"Problème de communication, arrêt du systeme");
		g_utf8_make_valid(str,strlen(str));
		line = TRUE;
	}
	else if (etat_com == 0 && line == TRUE){
		str = strcat(str,"\nProblème de communication, arrêt du système");
		g_utf8_make_valid(str,strlen(str));
	}
	if (line == FALSE){
		str = strcpy(str,"Rien à signaler");
		g_utf8_make_valid(str,strlen(str));
	}

	/* Mise a jour des valeurs à afficher */
	GtkTextBuffer *buf = gtk_text_buffer_new(NULL);	//Creation du buffer
	gtk_text_buffer_set_text(buf,str,strlen(str));				// Ecriture du texte dans le buffer
	gtk_text_view_set_buffer(GTK_TEXT_VIEW(widget),buf);	// Affichage du buffer dans le bon widget

	free(str);

	return TRUE;
}

/**
     * \fn gboolean update_angle(GtkWidget *widget)
     * \brief  Fonction appelée par la fonction update_gui, qui actualise la valeur de l'angle
	 * \param GTKWidget
*/
gboolean update_angle(GtkWidget *widget){
	char *str;

	if (widget == NULL) return FALSE;

	str = malloc(16);
	snprintf(str,16,"%f",angle);

	/* Mise a jour des valeurs à afficher */
	GtkTextBuffer *buf = gtk_text_buffer_new(NULL);	//Creation du buffer
	gtk_text_buffer_set_text(buf,str,strlen(str));				// Ecriture du texte dans le buffer
	gtk_text_view_set_buffer(GTK_TEXT_VIEW(widget),buf);	// Affichage du buffer dans le bon widget

	free(str);

	return TRUE;
}

/**
     * \fn gboolean update_vit_ang(GtkWidget *widget)
     * \brief  Fonction appelée par la fonction update_gui, qui actualise la valeur de la vitesse angulaire
	 * \param GTKWidget
*/
gboolean update_vit_ang(GtkWidget *widget){
	char *str;

	if (widget == NULL) return FALSE;

	str = malloc(16);
	snprintf(str,16,"%f",vit_ang);

	/* Mise a jour des valeurs à afficher */
	GtkTextBuffer *buf = gtk_text_buffer_new(NULL);
	gtk_text_buffer_set_text(buf,str,strlen(str));
	gtk_text_view_set_buffer(GTK_TEXT_VIEW(widget),buf);

	free(str);

	return TRUE;
}


/**
     * \fn gboolean update_vit_lin(GtkWidget *widget)
     * \brief  Fonction appelée par la fonction update_gui, qui actualise la valeur de la vitesse linéaire 
	 * \param GTKWidget
*/
gboolean update_vit_lin(GtkWidget *widget){
	char *str;

	if (widget == NULL) return FALSE;

	str = malloc(16);
	snprintf(str,16,"%f",vit_lin);

	/* Mise a jour des valeurs à afficher */
	GtkTextBuffer *buf = gtk_text_buffer_new(NULL);
	gtk_text_buffer_set_text(buf,str,strlen(str));
	gtk_text_view_set_buffer(GTK_TEXT_VIEW(widget),buf);

	free(str);

	return TRUE;
}

/**
     * \fn gboolean update_couple(GtkWidget *widget)
     * \brief  Fonction appelée par la fonction update_gui, qui actualise la valeur de la consigne de couple 
	 * \param GTKWidget
*/
gboolean update_couple(GtkWidget *widget){
	char *str;

	if (widget == NULL) return FALSE;

	str = malloc(16);
	snprintf(str,16,"%f",couple);

	/* Mise a jour des valeurs à afficher */
	GtkTextBuffer *buf = gtk_text_buffer_new(NULL);
	gtk_text_buffer_set_text(buf,str,strlen(str));
	gtk_text_view_set_buffer(GTK_TEXT_VIEW(widget),buf);

	free(str);

	return TRUE;
}

/**
     * \fn gboolean update_bat_lvl(GtkWidget *widget)
     * \brief  Fonction appelée par la fonction update_gui, qui actualise la valeur du niveau de batterie
	 * \param GTKWidget
*/
gboolean update_bat_lvl(GtkWidget *widget){
	char *str;
	char *str2;

	if (widget == NULL) return FALSE;

	str = malloc(16);
	str2 = malloc(16);
	sprintf(str,"%d",bat_lvl);
	sprintf(str2," %c",'%');
	str = strcat(str,str2);

	/* Mise a jour des valeurs a afficher */
	GtkTextBuffer *buf = gtk_text_buffer_new(NULL);
	gtk_text_buffer_set_text(buf,str,strlen(str));
	gtk_text_view_set_buffer(GTK_TEXT_VIEW(widget),buf);

	free(str);

	return TRUE;
}

/**
     * \fn gboolean update_log(GtkWidget *widget)
     * \brief Fonction appelée par la fonction update_gui, qui actualise le contenu du log
	 * \param GTKWidget
*/

gboolean update_log(GtkWidget *widget){
	
	gchar lecture[MAX_SIZE];
	int len,taille;
	unsigned char string[MAX_SIZE];

	compteur++;
	// La variable c permet d'éxécuter la fonction une fois sur deux, sans cela l'écriture dans le text_view n'a pas le temps de se faire
	if(compteur==2 && pass == 1){

		/* Lire un message du client */
	 	len = read(sckt_log, string, MAX_SIZE); 

		if(len > 0){
			taille=strlen((char*)string);
			memcpy(lecture,string,taille);
			
			GtkTextIter iter_end;	// Ces marqueurs servent de repaire pour le début et la fin du texte dans le buffer
			GtkTextIter iter_start;
			GtkTextBuffer *buf = gtk_text_buffer_new(NULL);

			if (widget == NULL) return FALSE;

			/* Recupération du buffer de l'espace de texte affichant le log */
			buf = gtk_text_view_get_buffer(GTK_TEXT_VIEW(widget));
			gtk_text_buffer_get_start_iter(buf,&iter_start);
			gtk_text_buffer_get_end_iter(buf,&iter_end);
			/* Possibilité grâce à la ligne suivante de supprimer le contenu de la fenêtre pour n'afficher que les dernière lignes de log reçues par le socket (retard par rapport au temps réel); */
			//gtk_text_buffer_delete(buf,&iter_start,&iter_end);

			/* Ecriture du log dans le fenetre dediée */
			gtk_text_buffer_insert(buf, &iter_end, g_utf8_make_valid (lecture,strlen(lecture)), -1);

			compteur=0;
		}
	}
	return TRUE;
}



/**
     * \fn gboolean update_etat_communication(GtkWidget *widget)
     * \brief Fonction appelée par la fonction update_gui, qui actualise la valeur de l'angle
	 * \param GTKWidget
*/
gboolean update_etat_communication(GtkWidget *widget){
	char *str;

	if (widget == NULL) return FALSE;

	str = malloc(16);
	if (etat_com==1){
		snprintf(str,16,"Oui");
	}else if (etat_com==0){
		snprintf(str,16,"Non");
	}
	
	/* Mise a jour des valeurs à afficher */
	GtkTextBuffer *buf = gtk_text_buffer_new(NULL);
	gtk_text_buffer_set_text(buf,str,strlen(str));
	gtk_text_view_set_buffer(GTK_TEXT_VIEW(widget),buf);

	free(str);

	return TRUE;
}

/**
     * \fn gboolean update_arret(GtkWidget *widget)
     * \brief Fonction appelée par la fonction update_gui, qui actualise la valeur de l'angle 
	 * \param GTKWidget
*/
gboolean update_arret(GtkWidget *widget){
	char *str;

	if (widget == NULL) return FALSE;

	str = malloc(16);
	if (arret==1){
		snprintf(str,16,"Oui");
	}else if (arret==0){
		snprintf(str,16,"Non");
	}
	
	/* Mise a jour des valeurs à afficher */
	GtkTextBuffer *buf = gtk_text_buffer_new(NULL);
	gtk_text_buffer_set_text(buf,str,strlen(str));
	gtk_text_view_set_buffer(GTK_TEXT_VIEW(widget),buf);

	free(str);

	return TRUE;
}

/**
     * \fn gboolean update_presence_user(GtkWidget *widget)
     * \brief Fonction appelée par la fonction update_gui, qui actualise la valeur de l'angle
	 * \param GTKWidget
*/
gboolean update_presence_user(GtkWidget *widget){
	char *str;

	if (widget == NULL) return FALSE;

	str = malloc(16);
	if (presence==1){
		snprintf(str,16,"Oui");
	}else if (presence==0){
		snprintf(str,16,"Non");
	}
	
	/* Mise a jour des valeurs à afficher */
	GtkTextBuffer *buf = gtk_text_buffer_new(NULL);
	gtk_text_buffer_set_text(buf,str,strlen(str));
	gtk_text_view_set_buffer(GTK_TEXT_VIEW(widget),buf);

	free(str);

	return TRUE;
}

/**
     * \fn gchar * g_utf8_make_valid (const gchar *str,gssize len);
     * \brief Librairie de validation de UTF-8
     */

/* If the provided string is valid UTF-8, return a copy of it. If not, return a copy in which bytes that could not be interpreted as valid Unicode are replaced with the Unicode replacement character (U+FFFD) */
gchar * g_utf8_make_valid (const gchar *str,gssize len)
{
  GString *string;
  const gchar *remainder, *invalid;
  gsize remaining_bytes, valid_bytes;

  g_return_val_if_fail (str != NULL, NULL);

  if (len < 0)
    len = strlen (str);

  string = NULL;
  remainder = str;
  remaining_bytes = len;

  while (remaining_bytes != 0) 
    {
      if (g_utf8_validate (remainder, remaining_bytes, &invalid)) 
	break;
      valid_bytes = invalid - remainder;
    
      if (string == NULL) 
	string = g_string_sized_new (remaining_bytes);

      g_string_append_len (string, remainder, valid_bytes);
      /* append U+FFFD REPLACEMENT CHARACTER */
      g_string_append (string, "\357\277\275");
      
      remaining_bytes -= valid_bytes + 1;
      remainder = invalid + 1;
    }
  
  if (string == NULL)
    return g_strndup (str, len);
  
  g_string_append (string, remainder);
  g_string_append_c (string, '\0');

  g_assert (g_utf8_validate (string->str, -1, NULL));

  return g_string_free (string, FALSE);
}
	

/**
     * \fn void update_GUI (AppWidgets* app)
     * \brief Fonction principale, appelée périodiquement 
 	*  initialise la connexion socket et appelle les fonctions qui mettent à jour l'affichage 
     */
void update_GUI (AppWidgets* app){
	
	//Première initialisation
	if(pass==0){
		printf("Attendre de la connexion de socket avec programme temps réel\n");
		sckt_log = init_socket(PORT_LOG);
		sckt_gui = init_socket(PORT_GUI);
		pass=1;
	}else{
		read_socket_values();
		update_angle(app->valeur_angle);
		update_vit_lin(app->vitesse_lin);
		update_vit_ang(app->vitesse_angulaire);
		update_couple(app->couple);
		update_bat_lvl(app->bat_lvl);
		update_info(app->info);
		update_log(app->log);
		update_etat_communication(app->etat_communication);
		update_arret(app->arret);
		update_presence_user(app->presence_user);
	}
}

