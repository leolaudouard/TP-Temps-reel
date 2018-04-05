/**
 * \file beta.cpp
 * \brief Source de la classe Beta
 * \author INSA Toulouse
 * \version 1.0
 * \date 25 Juillet 2017
 *
 * Implémentation de la classe Beta
 *
 */

#include "../inc/beta.h"

/**
     * \fn Constructeur de classe
     * \brief Constructeur de l'objet Beta.
     */
Beta::Beta(){
	_beta = BETA_INITIAL  ;
}

/**
     * \fn void set_beta(float beta)
     * \brief Mise à jour de la beta de l'objet Beta.
     * \param beta Nouvelle valeur de la beta de segway
     */
void Beta :: set_beta(float beta){
	this->_beta = beta;
}

/**
     * \fn float beta()
     * \brief Accesseur de la valeur _beta  de l'objet Beta.
     * \return _beta
     */
float Beta :: beta(){
	return _beta;
}

/**
     * \fn Destructeur de classe
     * \brief Destructeur de l'objet Beta.
     */
Beta :: ~Beta(void){}

