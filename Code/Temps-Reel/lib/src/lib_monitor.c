/**
 * \file lib_monitor.c
 * \author INSA Toulouse
 * \version 1.0
 * \date 20 Mai 2017
 */

#include "../inc/lib_monitor.h"

//declaration 
RT_TASK write_log;
RTIME begin_time;
RT_QUEUE log_queue;
FILE* log_file;
void * buf;
int recording_enabled;
int sckt_log;
/**
     * \fn void Write_log(void *arg)
     * \brief 		Function used by the TASK write_log in order to write events to a file. The periodicity is 1s. 
	 * The function reads every message availible in the queue and parse them into the file log.xenolog
	 * Warning : This task goes into secondary mode (because of asprintf and fflush())
	 * \param void *arg
	 * \return 1 if the recording is successfully launched, else 0.
     */
void Write_log(void *arg)
{

	rt_printf("WRITE LOG\n");
	//sckt_log = init_socket(PORT_LOG);

	rt_task_set_periodic(NULL, TM_NOW, SECENTOP);
	while (1) {
		rt_task_wait_period(NULL);

		//Opening log file
		log_file = fopen("log.xenolog","w");
		if(log_file == NULL){
				rt_printf("ERROR : Couldn't open log_file\n");
				//ret_value =0;
		}
			if(log_file != NULL){
				int err = sizeof(message_log);
				while(err == sizeof(message_log)){
					message_log m;
					err = rt_queue_read(&log_queue,&m,sizeof(message_log),SECENTOP / 10000);

					if(err == sizeof(message_log)){
						char * line;
						if(m.type == MUTEX){
							asprintf(&line,"[%d ms] [MUTEX] %s: mutex: %s => %s\n", m.time, m.task_name,m.sarg0,  m.event);
						}
						else if (m.type == SEM){
							asprintf(&line, "[%d ms] [SEM] %s: sem: %s => %s (%d waiters)\n", m.time, m.task_name, m.sarg0, m.event, m.iarg0);
						}
						else if (m.type == TASK){
							asprintf(&line, "[%d ms] [TASK] %s => %s (prio : %d)\n", m.time, m.task_name,  m.event, m.iarg0, m.iarg1);
						}
						else if (m.type == TASK_DELETE){
							asprintf(&line, "[%d ms] [TASK] %s => %s by %s (prio : %d)\n", m.time, m.task_name, m.event,m.sarg0, m.iarg0);
						}
						//rt_printf("Ecriture Log : %s!\n", line);
						rt_fprintf(log_file,"%s",line);
						write(sckt_log,line,strlen(line));
						fflush(log_file);
					}else{
						//do nothing
					}
				}
			}else{
				//rt_printf("Log : Error, can't access to log file\n");
			}
		//flock(fileno(log_file),LOCK_UN);
		//fclose(log_file);
	}
}


/**
     * \fn int init_recording()
     * \brief 	Initialize and start the recording of events (Tasks, semaphores and mutexes)
	 * \return 1 if the recording is successfully launched, else 0.
     */
int init_recording(){
	int ret_value = 0;

	// initialize reference time at the begining of the log
	begin_time =  rt_timer_ticks2ns(rt_timer_read());
	sckt_log = init_socket(PORT_LOG);
	// creates queue for the messages
	int err = 0;
	err = rt_queue_create(&log_queue, "Log Queue", sizeof(message_log)*25600, /*15000*/Q_UNLIMITED, Q_FIFO);
	if (err<0){
		switch(err){
			case (-EINVAL):
				rt_printf("rt_queue_create error: invalid mode or poolsize is 0\n");
				break;
			case (-ENOMEM):
				rt_printf("rt_queue_create error: system fails to get memory \n");
				break;
			case (-EEXIST):
				rt_printf("rt_queue_create error: name conflict \n");
				break;
			case (-EPERM):
				rt_printf("rt_queue_create error: called by asynchronous context \n");
				break;
		}
	}
	else rt_printf("Queue créée avec succès\n");
	
	// spawns the task that write to the log file
	if (rt_task_spawn(&write_log, /* task descriptor */
			"Log Writter", /* name */
			0           /* 0 = default stack size */,
			20          /* priority */,
			T_JOINABLE, /* needed to call rt_task_join after */
			&Write_log,      /* entry point (function pointer) */
			NULL        /* function argument */) != 0)
		{
			rt_printf("rt_task_spawn error\n");
			ret_value=0;
		}else{
			recording_enabled = 1;
			ret_value = 1;
			rt_printf("Started recording events\n");
		}
		return ret_value;
}


/**
     * \fn void stop_recording()
     * \brief Stop the event collection. Delete the queue, the task and then closes the log file.
     */
void stop_recording(){
		recording_enabled = 0;
		rt_queue_delete(&log_queue);
		rt_task_delete(&write_log);
		fclose(log_file);
		rt_printf("Stopped Recording events\n");
}

/**
     * \fn int write_in_queue(RT_QUEUE *msg_queue, void * buf, int size) 
     * \brief 	Writes a message into a queue
	 * \param RT_QUEUE *msg_queue : the address of the queue
	 * \param void * data : the address of message
	 * \param int size : the size of the data
	 * \return the error code return by the rt_queue
     */
int write_in_queue(RT_QUEUE *msg_queue, void * buf, int size) {
	int err=0;

	err = rt_queue_write(msg_queue,buf,size,Q_NORMAL);
	if (err<0){
		switch(err){
			case (-ENOMEM):
				rt_printf("rt_queue_send1 error limit exceeded %d\n",err);
				break;
			case (-EINVAL):
				rt_printf("rt_queue_send1 error: first argument is not a message queue descriptor or invalid mode or buf is NULL %d\n",err);
				break;
		}
	}
    return err;
}


/**
     * \fn int get_time_ms()
     * \brief 	Get the time between the start of recording and when it is called
	 * \return time in milliseconds
     */
int get_time_ms(){
	RTIME time = (rt_timer_ticks2ns(rt_timer_read())-begin_time)/1000000;
	return (int)time;
}

/**
     * \fn int get_time_us()
     * \brief 	Get the time between the start of recording and when it is called
	 * \return time in microseconds
     */
int get_time_us(){
	RTIME time = (rt_timer_ticks2ns(rt_timer_read())-begin_time)/1000;
	return (int)time;
}


/**
     * \fn void init_message(message_log * m, message_type type, int time, const char * task_name, const char * event, const char * sarg0, const char * sarg1, const char * sarg2, int iarg0, int iarg1)
     * \brief 		Builds a message to be sent to the queue
	 * \param    message_log * m : address of the message
	 * \param    int time : time of the creation (in ms)
	 * \param    char * task_name : name of the task
	 * \param    char * event : the event that created that message
	 * \param    char * sarg0 : generic string
	 * \param    char * sarg1 : generic string
	 * \param    char * sarg2 : generic string
	 * \param    int iarg0 : generic integer
	 * \param   int iarg1 : generic integer
     */

void init_message(message_log * m, message_type type, int time, const char * task_name, const char * event, const char * sarg0, const char * sarg1, const char * sarg2, int iarg0, int iarg1){

	m->type = type;
	m->time = time;
	m->iarg0 = iarg0;
	m->iarg1 = iarg1;
	if(sarg0 != NULL){
		memcpy(m->sarg0, sarg0, SARG_SIZE);
	}
	if(sarg1 != NULL){
		memcpy(m->sarg1, sarg1,SARG_SIZE);
	}
	if(sarg2 != NULL){
		memcpy(m->sarg2, sarg2, SARG_SIZE);
	}
	if(task_name != NULL){
		memcpy(m->task_name, task_name, SARG_SIZE);
	}
	if(event != NULL){
		memcpy(m->event, event, SARG_SIZE);
	}
}


/**
     * \fn void log_mutex_generic(RT_MUTEX * mut, const char * event)
     * \brief 		generic method to log en event regarding a mutex
	 * \param   RT_MUTEX * mut : the mutex
	 * \param   char * event : the event (waiting, acquired, released... )
     */
void log_mutex_generic(RT_MUTEX * mut, const char * event){
	if(recording_enabled){
		message_log m;
		RT_TASK_INFO task_info;
		RT_MUTEX_INFO mut_info;
		rt_task_inquire(NULL, &task_info);
		rt_mutex_inquire(mut, &mut_info);

		init_message(&m, MUTEX, get_time_ms(), task_info.name, event, mut_info.name,NULL, NULL, 0,0);
		write_in_queue(&log_queue,&m,sizeof(message_log));
	}
}


/**
     * \fn log_wait_for_mutex(RT_MUTEX  * mut)
     * \brief 		logs when the task waits for a mutex, it must be written just before the rt_mutex_acquire();
	 * \param   RT_MUTEX * mut : the mutex
     */
void log_wait_for_mutex(RT_MUTEX  * mut){
	log_mutex_generic(mut, "waiting");
}


/**
     * \fn log_mutex_acquired(RT_MUTEX * mut)
     * \brief 	logs when the task has acquired a mutex, it must be written just after the rt_mutex_acquire();
	 * \param   RT_MUTEX * mut : the mutex
     */
void log_mutex_acquired(RT_MUTEX * mut){
	log_mutex_generic(mut, "acquired");
}


/**
     * \fn log_mutex_released(RT_MUTEX * mut)
     * \brief 	logs when the task releases a mutex, it must be written just after the rt_mutex_release();
	 * \param  RT_MUTEX * mut : the mutex
     */
void log_mutex_released(RT_MUTEX * mut){
	log_mutex_generic(mut, "released");
}

/**
     * \fn log_sem_generic(RT_SEM * sem, const char * event)
     * \brief 	generic method to log en event regarding a semaphore
	 * \param  RT_SEM * sem : the semaphore
	 * \param  char * event : the event (waiting, entered, signaled... )
     */
void log_sem_generic(RT_SEM * sem, const char * event){
	if(recording_enabled){
		message_log m;
		RT_TASK_INFO task_info;
		RT_SEM_INFO sem_info;
		int err = rt_task_inquire(NULL, &task_info);
		rt_sem_inquire(sem, &sem_info);
		if(err ==0){
			init_message(&m, SEM, get_time_ms(), task_info.name, event, sem_info.name,NULL, NULL,sem_info.nwaiters,0);
			write_in_queue(&log_queue,&m,sizeof(message_log));
		}
	}

}

/**
     * \fn log_sem_waiting(RT_SEM * sem)
     * \brief 	logs when the task is waiting at a semaphore, it must be written just before the rt_sem_p();
	 * \param  RT_SEM * sem : the semaphore
     */
void log_sem_waiting(RT_SEM * sem){
	log_sem_generic(sem, "waiting");
}

/**
     * \fn log_sem_entered(RT_SEM * sem)
     * \brief 	logs when the task has entered a semaphore, it must be written just after the rt_sem_p()
	 * \param  RT_SEM * sem : the semaphore
     */
void log_sem_entered(RT_SEM * sem){
	log_sem_generic(sem, "entered");
}


/**
     * \fn log_sem_signaled(RT_SEM * sem)
     * \brief 	logs when the task signals a semaphore, it must be written just before the rt_sem_v()
	 * \param  RT_SEM * sem : the semaphore
     */
void log_sem_signaled(RT_SEM * sem){
	log_sem_generic(sem, "signaled");
}


/**
     * \fn log_task_generic(RT_TASK * task,  const char * event)
     * \brief 	generic method to log en event regarding a task
	 * \param  RT_TASK * task : the task
	 * \param char * event : the event (waiting, entered, signaled... )
     */
void log_task_generic(RT_TASK * task,  const char * event){
	if(recording_enabled){
		message_log m;
		RT_TASK_INFO self_task_info;
		int err = rt_task_inquire(task, &self_task_info);
		if(err==0){
			init_message(&m, TASK, get_time_ms(), self_task_info.name, event, NULL,NULL, NULL,self_task_info.prio, 0);
			write_in_queue(&log_queue,&m,sizeof(message_log));
		}
		else if (err == -EINVAL){
			rt_printf("NO VALID DESCRIPTOR OR PRIO INVALID\n");
		}
		else if (err == -EPERM){
			rt_printf("INVALID CONTEXT\n");
		}
	}
}

/**
	logs when a task begins, it must be called at the begining of the function
	no need to give a reference to the task
*/
/**
     * \fn void log_task_entered()
     * \brief 	logs when a task begins, it must be called at the begining of the function
     * \param no need to give a reference to the task
     */
void log_task_entered(){
	log_task_generic(NULL, "entered");
}


/**
     * \fn void log_task_new_iteration()
     * \brief 	logs when a task begins a new iteration, it must be called after the beginning of the function's loop
     * \param no need to give a reference to the task
     */
void log_task_new_iteration(){
	log_task_generic(NULL, "new iteration");
}


/**
     * \fn void log_task_ended()
     * \brief logs when a task ends, it must be called at the end of the function
     * \param no need to give a reference to the task
     */
void log_task_ended(){
	log_task_generic(NULL, "ended");
}

/**
     * \fn void log_task_deleted(RT_TASK * task)
     * \brief 	logs when a task is deleted, it must be called at before the rt_delete_task()
	 * \param RT_TASK * task : address of the task that is deleted
     */
void log_task_deleted(RT_TASK * task){

	if(recording_enabled){
		message_log m;
		RT_TASK_INFO self_task_info;
		RT_TASK_INFO param_task_info;
		int err_self = rt_task_inquire(NULL, &self_task_info);
		if(err_self==0){
			int err_param = rt_task_inquire(task, &param_task_info);
			if(err_param==0){
				init_message(&m, TASK_DELETE, get_time_ms(), param_task_info.name, "deleted", self_task_info.name,NULL, NULL,param_task_info.prio, 0);
				write_in_queue(&log_queue,&m,sizeof(message_log));
			}
		}
	}
}
