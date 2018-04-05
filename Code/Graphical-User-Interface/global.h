#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#ifndef _GNU_SOURCE
#define _GNU_SOURCE 1
#endif

/* Ports utilisés pour la communication socket */
#define PORT_GUI 8000 
#define PORT_LOG 8001

/*Taille maximum en octets du string lu sur le socket_log, contenant des lignes de log */
#define MAX_SIZE 1024 

/*Include */
#include <gtk/gtk.h>
#include <glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gui.h"
#include <malloc.h>
#include <errno.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/file.h>

/* Declaration des variables partagees*/
/* Ces variables contiennent les informations du Gyropode à afficher sur le GUI */
 double angle, vit_ang, vit_lin, couple;
 int bat_lvl, etat_com, arret, presence;

 int sckt_gui, sckt_log;	// Identifiants des sockets, récupérés lors de l'appel à la fonction socket(...)
 int pass,compteur;	

/* Structure contenant les differents Widgets a afficher */
typedef struct {
	GtkWidget *mywindow;
	GtkWidget *valeur_angle;
	GtkWidget *vitesse_angulaire;
	GtkWidget *vitesse_lin;
	GtkWidget *couple;
	GtkWidget *bat_lvl;
	GtkWidget *etat_communication;
	GtkWidget *arret;
	GtkWidget *presence_user;
	GtkWidget *scroll;
	GtkWidget *log;
	GtkWidget *info;
	GtkWidget *top;
} AppWidgets;

#endif
