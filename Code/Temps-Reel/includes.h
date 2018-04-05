/* 
 * File:   includes.h
 * Author: INSA Toulouse
 *
 * Created on 20 May 2017
 */



#ifndef INCLUDES_H
#define	INCLUDES_H

/*******************************************************************************/
/*                 ENTRER ICI L'ADRESSE IP DE LA MACHINE EXECUTANT LE GUI       */
#define ADRESSE_IP "10.32.1.80"
/*																									   									  */
/*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>     
#include <sys/mman.h>
#include <alchemy/task.h>
#include <alchemy/mutex.h>
#include <alchemy/sem.h>
#include <alchemy/timer.h>
#include <alchemy/queue.h>

#include <sys/socket.h>

#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>  

#include <time.h>
#include "global.h"
#include "fonctions.h"
#include <signal.h>
#include <string.h>
#include <malloc.h>

#include "lib/lib.h"

//#include <sys/types.h>
//#include <arpa/inet.h>
//#include <netinet/in.h>
//#include <netinet/tcp.h>
//#include <sys/file.h>
#endif	/* INCLUDES_H */
