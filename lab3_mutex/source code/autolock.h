#ifndef _AUTOLOCK_H_
#define _AUTOLOCK_H_

class CAutoLock
{
public:
	CAutoLock(mylock& lock,int nLock=1);
	~CAutoLock(void);

private:
	mylock _lock;

	int _isLock;
};

#endif
