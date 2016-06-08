#ifndef _LOCK_H_
#define _LOCK_H_

class mylock
{
public:
	mylock(void);
	~mylock(void);

public:
	void lock();
	void unlock();

private:
	pthread_mutex_t _mutex;
};

#endif
