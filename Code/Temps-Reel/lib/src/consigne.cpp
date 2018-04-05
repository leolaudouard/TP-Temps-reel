/**
 * \file consigne.cpp
 * \brief Source de la classe Consigne
 * \author INSA Toulouse
 * \version 1.0
 * \date 25 Juillet 2017
 *
 * Implémentation de la classe Consigne
 *
 */

#include "../inc/consigne.h"

/**
     * \fn Constructeur de classe
     * \brief Constructeur de l'objet Battery.
     */
Consigne::Consigne(){
	_consigne = CONSIGNE_INITIAL ;
}

/**
     * \fn void set_consigne(float consigne)
     * \brief Mise à jour de la consigne de l'objet Consigne.
     * \param consigne Nouvelle valeur de la consigne de l'asservissement 
     */
void Consigne :: set_consigne(float consigne){
	this->_consigne = consigne;
}

/**
     * \fn float consigne()
     * \brief Accesseur de la valeur _consigne  de l'objet Consigne.
     * \return _consigne
     */
float Consigne :: consigne(){
	return _consigne;
}

/**
     * \fn Destructeur de classe
     * \brief Destructeur de l'objet Consigne.
     */
Consigne :: ~Consigne(void){}

