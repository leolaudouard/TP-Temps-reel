/**
 * \file lib_serial.h
 * \author INSA Toulouse
 * \version 1.0
 * \date 20 Mai 2017
 */

#ifndef LIB_SERIAL_H
#define LIB_SERIAL_H

#include "../../includes.h"

#define MESSAGE_SERIAL_LENGTH 37
#define MESSAGE_TIMEOUT_NS 2000

typedef struct
{
  char label;
  float value;
} message_serial;

typedef struct
{
	char label;
	float fval;
	int ival;
} message_stm;

extern int lost_com;

#ifdef	__cplusplus
extern "C" {
#endif

  int init_serial();
  message_serial* read_from_serial();
  int send_float_to_serial(float fl_value, char label);
  int send_int_to_serial(int int_value, char label);
  int bytes_to_array_of_message(unsigned char * mesg, message_serial *m);
  float bytes_to_float(unsigned char * bytes);
  int close_serial();

  void write_trame_to_data(message_serial *m);
  
#ifdef	__cplusplus
}
#endif

#endif
