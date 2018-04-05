/**
 * \file vitesse.h
 * \brief Déclaration de la classe Beta.
 * \author INSA Toulouse
 * \version 1.0
 * \date 25 Juillet 2017
 */

#ifndef BETA_H
#define BETA_H

#ifdef __cplusplus
extern "C" {
#endif

#include "constantes.h"

class Beta{
	private :
		float _beta;
	public :
		Beta();
		void set_beta(float beta);
		float beta();
		~Beta(void);
};
#ifdef __cplusplus
}
#endif
#endif
