/**
 * \file angles.cpp
 * \brief Source de la classe Angles
 * \author INSA Toulouse
 * \version 1.0
 * \date 25 Juillet 2017
 *
 * Implémentation de la classe Angles
 *
 */

#include "../inc/angles.h"

/**
     * \fn Constructeur de classe
     * \brief Constructeur de l'objet Angles.
     */
Angles::Angles(){
	_angle = ANGLE_INITIAL;
	_vitesse_ang = VITESSE_ANG_INITIAL;
}

/**
     * \fn void set(float theta, float thetap)
     * \brief Mise à jour des données décrit par un objet Angles.
     * \param theta Nouvelle valeur de l'angle (entre -20.0 et 20.0)
     * \param thetap Nouvelle valeur de la vitesse angulaire
     */
void Angles :: set(float theta, float thetap){
	this->_angle = theta;
	this->_vitesse_ang= thetap;
}

/**
     * \fn void set_angle(float theta)
     * \brief Mise à jour de l'angle de l'objet Angles.
     * \param theta Nouvelle valeur de l'angle (entre -20.0 et 20.0)
     */
void Angles :: set_angle(float theta){
	this->_angle = theta;
}

/**
     * \fn void set_vitesse_ang(float thetap)
     * \brief Mise à jour de la vitesse angulaire de l'objet Angles.
     * \param theta Nouvelle valeur de la vitesse angulaire
     */
void Angles :: set_vitesse_ang(float thetap){
	this->_vitesse_ang = thetap;
}

/**
     * \fn float angle()
     * \brief Accesseur de la valeur _angle  de l'objet Angles.
     * \return _angle
     */
float Angles :: angle(){
	return _angle;
}

/**
     * \fn float vitesse_ang()
     * \brief Accesseur de la valeur _vitesse_ang  de l'objet Angles.
     * \return _vitesse_ang
     */
float Angles :: vitesse_ang(){
	return _vitesse_ang;
}

/**
     * \fn Destructeur de classe
     * \brief Destructeur de l'objet Angles.
     */
Angles :: ~Angles(void){}

 
