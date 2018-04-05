 /**
  * \file lib_msg.GUI.h
  * \author INSA Toulouse
  * \version 1.0
  * \date 20 Juillet 2017
  */

#ifndef LIB_MSG_GUI_H
#define LIB_MSG_GUI_H

#include "../../includes.h"
#define PORT_GUI 8000 /* server's port */
#define PORT_LOG 8001

#ifdef	__cplusplus
extern "C" {
#endif
int init_socket(int port);
void add_info_float(unsigned char * str, char label, float data,int *indice);
void add_info_int(unsigned char * str, char label, int data,int *indice);
void send_trame(int sock, unsigned char * msg,int* indice);

#ifdef	__cplusplus
}
#endif
#endif
