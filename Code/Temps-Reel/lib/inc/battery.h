/**
 * \file battery.h
 * \brief Déclaration de la classe Battery.
 * \author INSA Toulouse
 * \version 1.0
 * \date 25 Juillet 2017
 */

#ifndef BATTERY_H
#define BATTERY_H

#ifdef __cplusplus
extern "C" {
#endif

#include "constantes.h"

class Battery{
	private :
		int _level;
	public :
		Battery();
		void set_level(int level);
		int level();
		~Battery(void);
};
#ifdef __cplusplus
}
#endif
#endif
