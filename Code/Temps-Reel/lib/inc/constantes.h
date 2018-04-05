/**
 * \file constantes.h
 * \brief Déclaration des constantes du projet
 * \author INSA Toulouse
 * \version 1.0
 * \date 27 Juin 2017
 *
 * Déclaration des constantes du projet
 *
 */

#ifndef CONSTANTES_H
#define CONSTANTES_H

/*! \def BATTERY_LEVEL_UNKNOWN
    \brief Valeur décrivant un niveau inconnu de la batterie
*/
#define BATTERY_LEVEL_UNKNOWN               (-1)

/*! \def BATTERY_WARNING_UNKNOWN
    \brief Valeur décrivant un état inconnu de la batterie
*/
#define BATTERY_WARNING_UNKNOWN               (-1)

/*! \def VITESSE_ANG_INITIAL
    \brief Valeur décrivant la valeur initial des consignes de commandes 
*/
#define CONSIGNE_INITIAL                    (0.0)

/*! \def ANGLE_INITIAL
    \brief Valeur décrivant la valeur initial de angle
*/
#define ANGLE_INITIAL                       (0.0)

/*! \def VITESSE_ANG_INITIAL
    \brief Valeur décrivant la valeur initial de vitesse angulaire
*/
#define VITESSE_ANG_INITIAL                 (0.0)

/*! \def DIRECTION_INITIAL
    \brief Valeur décrivant la valeur initial de angle de direction
*/
#define DIRECTION_INITIAL                   (0.0)

/*! \def BETA_INITIAL    
    \brief Valeur décrivant la valeur initial de beta
*/
#define BETA_INITIAL                 (0.0)

/*********** etat_com **************/

/*! \def CONNECTE
    \brief Valeur décrivant la valeur de etat_com lorsque la communication fonctionne
*/
#define CONNECTE		                    (1)

/*! \def NON_CONNECTE
    \brief Valeur décrivant la valeur de etat_com lorsque la communication est rompue
*/
#define NON_CONNECTE		                (0)



#endif
