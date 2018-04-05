#ifndef _GUI_H_
#define _GUI_H_

#include "global.h"

/* Initialise le socket */
int init_socket();

/* Lit les valeurs reçues dans le buffer du socket */
void read_socket_values();

/* Actualise les infos communiquees a l'utilisateur */
gboolean update_info(GtkWidget *widget);

/* Fonction appelee par la fonction update_gui, qui actualise la valeur de l'angle */
gboolean update_angle(GtkWidget *widget);

/* Fonction appelee par la fonction update_gui, qui actualise la valeur de la vitesse angulaire*/
gboolean update_vit_ang(GtkWidget *widget);

/* Fonction appelee par la fonction update_gui, qui actualise la valeur de la vitesse lineaire */
gboolean update_vit_lin(GtkWidget *widget);

/* Fonction appelee par la fonction update_gui, qui actualise la valeur de la consigne de couple */
gboolean update_couple(GtkWidget *widget);

/* Fonction appelee par la fonction update_gui, qui actualise la valeur du niveau de batterie*/
gboolean update_bat_lvl(GtkWidget *widget);

/* Fonction appelee par la fonction update_gui, qui actualise le contenu du log */
gboolean update_log(GtkWidget *widget);

/* Fonction appelée par la fonction update_gui, qui actualise la valeur de l'angle */
gboolean update_etat_communication(GtkWidget *widget);

/* Fonction appelée par la fonction update_gui, qui actualise la valeur de l'angle */
gboolean update_arret(GtkWidget *widget);

/* Fonction appelée par la fonction update_gui, qui actualise la valeur de l'angle */
gboolean update_presence_user(GtkWidget *widget);

/* Fonction principale, appelée périodiquement 
 *  initialise la connexion socket et appelle les fonctions qui mettent à jour l'affichage */
void update_GUI (AppWidgets* app);

/* If the provided string is valid UTF-8, return a copy of it.
 * If not, return a copy in which bytes that could not be interpreted
 * as valid Unicode are replaced with the Unicode replacement character (U+FFFD) */
gchar * g_utf8_make_valid (const gchar *str,gssize len);

#endif
