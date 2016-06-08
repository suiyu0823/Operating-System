#include "main.h"
#include "autolock.h"

CAutoLock::CAutoLock(mylock& lock,int nLock)
{
	_lock=lock;
	if (nLock)
	{
		_lock.lock();		
	}
	_isLock=nLock;
}

CAutoLock::~CAutoLock(void)
{
	if (_isLock)
	{
		_lock.unlock();
	}
}

