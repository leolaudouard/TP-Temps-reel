#include "gui.h"
#include "global.h"

int main(int argc, char *argv[]){

	/* Variables partagees initialisation*/
	angle = 0.0f;
	vit_ang = 0.0f;
	vit_lin = 0.0f;
	couple = 0.0f;
	bat_lvl = -1;
	etat_com = 1;
	pass=0;				// Sert de flag pour notifier l'initialisation des sockets
	arret=0;
	compteur=0;						// Uitlisé par la fonction update_log
	presence=1;

	GtkBuilder *gtkBuilder;
	GError *err = NULL;
	AppWidgets *app = g_slice_new(AppWidgets);

	/* Initialisation de GTK+ */
	gtk_init(&argc, &argv);

	/* Creation de la fenetre */
	gtkBuilder = gtk_builder_new();

	/* Le builder construit le GUI depuis le fichier glade */
	if(0 == gtk_builder_add_from_file(gtkBuilder, "GUI.glade", &err)){
	fprintf(stderr, "Error adding build from file. Error: %s\n", err->message);
	}

	/* Liaison des Widget de la structure app avec ceux crees par le programme Glade  dans le fichier GUI.glade*/
	app->mywindow = GTK_WIDGET(gtk_builder_get_object(gtkBuilder, "mywindow"));
	app->valeur_angle = GTK_WIDGET(gtk_builder_get_object(gtkBuilder, "valeur_angle"));
	app->vitesse_angulaire = GTK_WIDGET(gtk_builder_get_object(gtkBuilder, "vitesse_angulaire"));
	app->vitesse_lin = GTK_WIDGET(gtk_builder_get_object(gtkBuilder, "vitesse_lin"));
	app->couple = GTK_WIDGET(gtk_builder_get_object(gtkBuilder, "couple"));
	app->bat_lvl = GTK_WIDGET(gtk_builder_get_object(gtkBuilder, "batterie_lvl"));
	app->scroll = GTK_WIDGET(gtk_builder_get_object(gtkBuilder, "scroll"));
	app->log = GTK_WIDGET(gtk_builder_get_object(gtkBuilder, "log"));
	app->info = GTK_WIDGET(gtk_builder_get_object(gtkBuilder, "info"));
	app->etat_communication= GTK_WIDGET(gtk_builder_get_object(gtkBuilder, "etat_communication"));
	app->arret= GTK_WIDGET(gtk_builder_get_object(gtkBuilder, "arret"));
	app->presence_user= GTK_WIDGET(gtk_builder_get_object(gtkBuilder, "presence_user"));

	/* Permet connecter les signaux declenches par les evenements avec les fonctions handlers */
	gtk_builder_connect_signals(gtkBuilder,NULL);

	/* Declaration des fonctions executees periodiquement */
	g_timeout_add_full(99,100,(GSourceFunc)update_GUI, (gpointer)app, NULL);

	/* Affichage */
	app->top = gtk_widget_get_toplevel(GTK_WIDGET(app->mywindow));
	gtk_widget_show(app->mywindow);	
	
	/* Boucle evenementielle */
	gtk_main();

	return 0;
}

