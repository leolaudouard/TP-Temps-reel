/**
 * \file consigne.h
 * \brief Déclaration de la classe Consigne.
 * \author INSA Toulouse
 * \version 1.0
 * \date 25 Juillet 2017
 */

#ifndef CONSIGNE_H
#define CONSIGNE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "constantes.h"

class Consigne{
	private :
		float _consigne;
	public :
		Consigne();
		void set_consigne(float consigne);
		float consigne();
		~Consigne(void);
};
#ifdef __cplusplus
}
#endif
#endif
