/* 
 * File:   lib_serial.c
 * Author: INSA TOULOUSE
 *
 * Created on 10-07 2017
 */

#include "../inc/lib_serial.h"

/**
	Initialize the serial port in order to send and receive messages through it
	@returns: the file descriptor, or -1 if there was an error. 
	(Retreiving the file descriptor is not necessary as it is not an argument of any function of the library)

*/

int uart0_filestream;

int init_serial(){

	uart0_filestream = open("/dev/ttyS0", O_RDWR | O_NOCTTY | O_NDELAY);		//Open in non blocking read/write mode
	if (uart0_filestream == -1)
	{
		//ERROR - CAN'T OPEN SERIAL PORT
		rt_printf("Error - Unable to open UART.  Ensure it is not in use by another application\n");
		exit(EXIT_FAILURE);
	}

	//Configuration of the serial port 115 520 Bauds
	struct termios options;
	tcgetattr(uart0_filestream, &options);
	options.c_cflag = B115200 | CS8 | CLOCAL | CREAD;		//<Set baud rate
	options.c_iflag = IGNPAR; // ignores bytes with bad parity
	options.c_oflag = 0;
	options.c_lflag = 0;
	tcflush(uart0_filestream, TCIFLUSH);
	tcsetattr(uart0_filestream, TCSANOW, &options);
	return uart0_filestream;
}

/**
	Takes an array of 7 bytes and tries to format it to a message structure

	@params unsigned char * mesg : an array of 7 bytes.
		the byte 0 should always be '<'
		the byte 1 should be an ascii char that is the label. It define what the data represent
		the bytes 2 to 5 are the float value
		the byte 6 should always be a '\n'
	@params message_serial * m : a pointer of a message. If there was an issue when reading the serial port
		the message takes 'e' (for error) as its label

	@returns 0 if there was an issue while reading the message else 1
*/ 

int bytes_to_array_of_message(unsigned char * mesg, message_serial *m){

	int ret =0;
	int i,y=0;

	for (y=0 ; y < 35 ; y = y+7 )
	{
		if(mesg[y] == '<' && mesg[y+6] == '\n' ){
			m[y/7].label = mesg[y+1];
			m[y/7].value = bytes_to_float(&mesg[y+2]);
			ret=1;
		}else{
			m[y/7].label = 'e';
			ret=0;
		}
	}
	return ret;
}

/**
	Converts a float into a byte sequence that is sent on the serial port 
	the message is '<'{tag}{the float encoded on 4 bytes}'\n

	@params float fl_value : The value to send
	@params char tag : the tag that define what the value represents.  
	
	@return 1 if the message was sent, else 0
*/
int send_float_to_serial(float fl_value, char label){
	
	unsigned char * msg = (unsigned char *) &fl_value; 
	int ret_val = 0;
	unsigned char tx_buffer[MESSAGE_SERIAL_LENGTH];
	unsigned char * p_tx_buffer;
 	p_tx_buffer=&tx_buffer[0];
	*p_tx_buffer++ = '<';
	*p_tx_buffer++ = label;
	*p_tx_buffer++ = msg[0];
	*p_tx_buffer++ = msg[1];
	*p_tx_buffer++ = msg[2];
	*p_tx_buffer++ = msg[3];
	*p_tx_buffer++ = '\n';

	if (uart0_filestream != -1){
		int count = write(uart0_filestream, &tx_buffer[0], (p_tx_buffer-&tx_buffer[0]));              //Filestream, bytes to write, number of bytes to write
		if (count < 0){
			rt_printf("UART TX error\n");
		}
		else{
			ret_val = 1;
		}	
	}
	return ret_val;
}

int send_int_to_serial(int int_value, char label){

	float tampon= int_value;
	unsigned char * msg = (unsigned char *) &tampon; 
	int ret_val = 0;
	unsigned char tx_buffer[MESSAGE_SERIAL_LENGTH];
	unsigned char * p_tx_buffer;
 	p_tx_buffer=&tx_buffer[0];
		
	*p_tx_buffer++ = '<';
	*p_tx_buffer++ = label;
	*p_tx_buffer++ = msg[0];
	*p_tx_buffer++ = msg[1];
	*p_tx_buffer++ = msg[2];
	*p_tx_buffer++ = msg[3];
	*p_tx_buffer++ = '\n';

	if (uart0_filestream != -1){
		int count = write(uart0_filestream, &tx_buffer[0], (p_tx_buffer-&tx_buffer[0]));              //Filestream, bytes to write, number of bytes to write
		if (count < 0){
			rt_printf("UART TX error\n");
		}
		else{
			ret_val = 1;
		}	
	}
	return ret_val;
}


/**
	Converts an array of 4 bytes into a float

	@params unsigned char * bytes : array of 4 bytes

	@return the float value
*/

float bytes_to_float(unsigned char * bytes){
	unsigned long value;
	value = (bytes[3] << 24) | (bytes[2] << 16) | (bytes[1] << 8) | (bytes[0]);
	return *(float *)&value;
}


/**
	Reads a message from the serial port. 
	The function is blocked until a suitable message is received. 
*/
int lost_com=0;

message_serial*  read_from_serial(){

	int message_length = 0;
	unsigned char message_buffer[256];
    static message_serial m[5];
    int i,j,pass = 0;
	long k=0; 
	int passtotal = 0;
	while (passtotal == 0)
	{
	    // Read up to 255 characters from the port if they are there
	    unsigned char rx_buffer[256];
	    int rx_length = read(uart0_filestream, (void*)rx_buffer,MESSAGE_SERIAL_LENGTH-j);		//Filestream, buffer to store in, number of bytes to read (max)
         k++;
        if (rx_length < 0)
		{
			//rt_printf("Error when checking for rx bytes\n");
		}
		else if (rx_length == 0)
		{
			//rt_printf("No data is available");
		}
		else
		{		
			lost_com=0;
			k=0;
		    for (i = 0; i<rx_length; i++) {
		        if (pass==1) {
		            if (rx_buffer[i]=='X'){
		                 pass= 0;
		                 passtotal=1;
		            }else{
		                message_buffer[j] = rx_buffer[i];
		                j++;
		            }
		        }                    
		        else if (rx_buffer[i]=='R' ){
		            j=0;
		            pass= 1;
		        }	
		    }                    
         }    //else

		if (k >10000){
			k=0;
			rt_printf("Perte de connexion\n");
			passtotal=1;
			lost_com=1;
		}
        
    }//while passtotal

    bytes_to_array_of_message(message_buffer, m);
    return m;
}


int close_serial(){
	return close(uart0_filestream);
}


void write_trame_to_data(message_serial *m){
    int i;
	for (i=0;i<5;i++){
		switch ((m+i)->label){
		case 'p':
			rt_mutex_acquire(&var_mutex_etat_angle, TM_INFINITE);
			etat_angle.set_angle((m+i)->value);
			rt_mutex_release(&var_mutex_etat_angle);

			break;
		case 's':
			rt_mutex_acquire(&var_mutex_etat_angle, TM_INFINITE);
           		etat_angle.set_vitesse_ang((m+i)->value); 
			rt_mutex_release(&var_mutex_etat_angle);

			break;
		case 'b':
			rt_mutex_acquire(&var_mutex_batterie, TM_INFINITE);
			batterie.set_level((int)((m+i)->value));
			rt_mutex_release(&var_mutex_batterie);

			break;
		case 'v':	
			rt_mutex_acquire(&var_mutex_beta, TM_INFINITE);
            beta.set_beta((m+i)->value); 
			rt_mutex_release(&var_mutex_beta);

			break;
		case 'u':
			rt_mutex_acquire(&var_mutex_presence_user, TM_INFINITE);
			presence_user = (int)(m+i)->value;
			rt_mutex_release(&var_mutex_presence_user);

			break;
              		rt_printf("Unknown message type : tag '%c'\n", (m+i)->label);
                }
        }
}

		


