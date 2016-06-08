#ifndef _MAIN_H_
#define _MAIN_H_

#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <time.h>
#include <fcntl.h>
#include <stddef.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/un.h>
//#include <event.h>
#include <pthread.h>
#include <linux/if_tun.h>
#include <signal.h>
#include <fcntl.h>
#include <errno.h>

using namespace std;

#define MACRO_GLOBAL_LOCK 1
#define MACRO_NODE_LOCK 1



#include "lock.h"
#include "autolock.h"
#include "stacklist.h"

#endif
