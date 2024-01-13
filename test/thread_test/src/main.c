/****************************************************************************************************/
/*                                           INCLUDE                                                */
/****************************************************************************************************/
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
/****************************************************************************************************/
/*                                           DEFINES                                                */
/****************************************************************************************************/
#define MY_THREAD_STACK_SIZE        (8 * 1024 * 1024)
/****************************************************************************************************/
/*                                           VARIABLES                                              */
/****************************************************************************************************/
pthread_mutex_t l_mutex;
pthread_cond_t l_condition;
int l_share_data = 0;
/****************************************************************************************************/
/*                                       STATIC FUNCTIONS                                           */
/****************************************************************************************************/

/****************************************************************************************************/
/*                                       PUBLIC FUNCTIONS                                           */
/****************************************************************************************************/
void *my_thread_start(void)
{
	pthread_mutex_lock(&l_mutex);

	while(1)
	{
		pthread_cond_wait(&l_condition, &l_mutex);
		l_share_data = 1;
		printf("My thread is awake, l_share_data = %d\n", l_share_data);
	}

	pthread_mutex_unlock(&l_mutex);

	return NULL;
}

int main(int argc, char *argv[])
{
	pthread_t my_tid;
	pthread_attr_t my_attr;

	pthread_mutex_init(&l_mutex, NULL);
	pthread_cond_init(&l_condition, NULL);

	pthread_attr_init(&my_attr);
	pthread_attr_setstacksize(&my_attr, MY_THREAD_STACK_SIZE);
	pthread_attr_setdetachstate(&my_attr, PTHREAD_CREATE_DETACHED);

	pthread_create(&my_tid, &my_attr, (void *)my_thread_start, NULL);
	
	pthread_attr_destroy(&my_attr);

	while (1)
	{
		sleep(1);
		pthread_cond_signal(&l_condition);
		pthread_mutex_lock(&l_mutex);
		l_share_data = 0;
		printf("After doing awake my_thread, l_share_data = %d\n", l_share_data);
		pthread_mutex_unlock(&l_mutex);
	}

	return 0;
}
