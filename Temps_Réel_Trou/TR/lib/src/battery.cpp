/**
 * \file battery.cpp
 * \brief Source de la classe Battery
 * \author INSA Toulouse
 * \version 1.0
 * \date 25 Juillet 2017
 *
 * Implémentation de la classe Angles
 *
 */

#include "../inc/battery.h"

/**
     * \fn Constructeur de classe
     * \brief Constructeur de l'objet Battery.
     */
Battery::Battery(){
	_level = BATTERY_LEVEL_UNKNOWN ;
}

/**
     * \fn void set_level(int level)
     * \brief Mise à jour du niveau de batterie de l'objet Battery.
     * \param level Nouvelle valeur du niveau (entre 0 - 100)
     */
void Battery :: set_level(int level){
	this->_level = level;
}

/**
     * \fn int level()
     * \brief Accesseur de la valeur _level  de l'objet Battery.
     * \return _level
     */
int Battery :: level(){
	return _level;
}

/**
     * \fn Destructeur de classe
     * \brief Destructeur de l'objet Battery.
     */
Battery :: ~Battery(void){}

