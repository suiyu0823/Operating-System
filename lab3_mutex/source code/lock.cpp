#include "main.h"
#include "lock.h"

mylock::mylock(void)
{
	pthread_mutex_init(&_mutex,NULL);	
}

mylock::~mylock(void)
{
	pthread_mutex_destroy(&_mutex);
}

void mylock::lock()
{	
	pthread_mutex_lock(&_mutex);
}

void mylock::unlock()
{
	pthread_mutex_unlock(&_mutex);
}
