/**
 * \file angles.h
 * \brief Déclaration de la classe Angles.
 * \author INSA Toulouse
 * \version 1.0
 * \date 25 Juillet 2017
 */

#ifndef ANGLES_H
#define ANGLES_H

#ifdef __cplusplus
extern "C" {
#endif

#include "constantes.h"

class Angles{
	private :
		float _angle;
		float _vitesse_ang;
	public :
		Angles();
		void set(float theta, float thetap);
		void set_angle(float theta);
		void set_vitesse_ang(float thetap);
		float angle();
		float vitesse_ang();
		~Angles(void);
};

#ifdef __cplusplus
}
#endif
#endif
