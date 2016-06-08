#include "main.h"
#include "stacklist.h"

mylock lock;
CStackList list;

#define MAX_NODECOUNT 10000


//thread onass
void* threadOne(void* arg)
{
		int nValue=0;
		list.destory();
		printf("cur list count:%d\r\n",list.countList());
		while(list.countList()<=MAX_NODECOUNT)
		{
				nValue=rand();
				{
						CAutoLock _l(lock,MACRO_GLOBAL_LOCK);
						list.insert(nValue);
				}				
				usleep(100);
		}
		printf("cur list count:%d\r\n",list.countList());
}

//thread two
int tdtwo1stop=0;
void* threadTwo1(void* arg)
{
		int nValue=0;
		list.destory();
		tdtwo1stop=0;
		printf("cur list count:%d\r\n",list.countList());
		while(list.countList()<=MAX_NODECOUNT)
		{
				nValue=rand();
				{
						CAutoLock _l(lock,MACRO_GLOBAL_LOCK);
						list.insert(nValue);						                        
						//printf("cur list count:%d\r\n",list.countList());
				}
				usleep(100);
		}
		tdtwo1stop=1;
		printf("cur list count:%d\r\n",list.countList());
}

void* threadTwo2(void* arg)
{
		int nRndPos=0;
		int nValue=0;
		while(tdtwo1stop==0)
		{
				nRndPos=rand()%list.countList();
				{
						CAutoLock _l(lock,MACRO_GLOBAL_LOCK);
						nValue=list.getValue(nRndPos);
						//printf("pos %d value is %d lower than 0x200\r\n",nRndPos,nValue);
						if(nValue<0x200)
						{
								printf("pos %d value is %d lower than 0x200\r\n",nRndPos,nValue);
						}
				}
				usleep(100);

		}
		printf("cur list count:%d\r\n",list.countList());
}


//thread three
int tdthreestop=0;
void* threadThree1(void* arg)
{
		int nValue=0;
		list.destory();
		tdthreestop=0;

		while(1)
		{
				CAutoLock _l(lock,MACRO_GLOBAL_LOCK);
				{
						if(list.countList()>MAX_NODECOUNT)
						{
								break;
						}
						nValue=rand();		
						list.insert(nValue);
						printf("cur list count:%d\r\n",list.countList());
				}
				usleep(100);
		}
		tdthreestop=1;
		printf("cur list count:%d\r\n",list.countList());
}

void* threadThree2(void* arg)
{
		int nRndPos=0;
		int nValue=0;
		while(tdthreestop==0)
		{
				CAutoLock _l(lock,MACRO_GLOBAL_LOCK);
				{
						nRndPos=rand()%list.countList();
						
						nValue=rand();
						list.setValue(nRndPos,nValue);
						printf("pos %d new value is %d\r\n",nRndPos,nValue);
				}
				usleep(100);
		}
		printf("cur list count:%d\r\n",list.countList());
}

//thread four
int tdfourstop=0;
void* threadFour1(void*)
{
		int nValue=0;
		tdfourstop=0;
		printf("thread four start.\r\n");
		while(1)
		{
				CAutoLock _l(lock,MACRO_GLOBAL_LOCK);
				{
						if(list.countList()>MAX_NODECOUNT)
						{
								break;
						}
						nValue=rand();
						list.insert(nValue);
				}
				usleep(100);
		}
		tdfourstop=1;
		printf("thread four stop.\r\n");
}

void* threadFour2(void*)
{
		int nRndPos=0;
		int nValue=0;
		while(tdfourstop==0)
		{
				CAutoLock _l(lock,MACRO_GLOBAL_LOCK);
				{
						nRndPos=rand()%list.countList();
						nValue=list.getValue(nRndPos);
						printf("pos %d value is:%d\r\n",nRndPos,nValue);
				}
				usleep(100);
		}
}

void* threadFour3(void*)
{
		int nRndPos=0;
		int nValue=0;
		while(tdfourstop==0)
		{
				CAutoLock _l(lock,MACRO_GLOBAL_LOCK);
				{
						nRndPos=rand()%list.countList();
						
						nValue=rand();
						list.setValue(nRndPos,nValue);
						printf("pos %d new value is %d\r\n",nRndPos,nValue);
				}
				usleep(100);
		}
		printf("cur list count:%d\r\n",list.countList());
}

void* threadFour4(void*)
{
		int nRndPos=0;
		while(tdfourstop==0)
		{
				CAutoLock _l(lock,MACRO_GLOBAL_LOCK);
				{
						nRndPos=rand()%list.countList();
						list.remove(nRndPos);						
						printf("remove pos %d\r\n",nRndPos);
				}
		}
}

int main()
{
		srand(time(NULL));

		int bExit=0;
		int i;
		while(!bExit)
		{
				printf("1.one thread\r\n");
				printf("2.two thread\r\n");
				printf("3.ten thread\r\n");
				printf("4.100th thread\r\n");
				printf("0.exit\r\n");

				int nOption=0;
				scanf("%d",&nOption);

				switch(nOption)
				{
						case 0:
								bExit=1;
								break;
						case 1:
								pthread_t tid;
								pthread_create(&tid,NULL,threadOne,NULL);
								break;
						case 2:
								pthread_t tid1,tid2;
								pthread_create(&tid1,NULL,threadTwo1,NULL);
								pthread_create(&tid2,NULL,threadTwo2,NULL);
								break;
						case 3:
								pthread_t tid31[2],tid32[8];
								for(i=0;i<1;i++)
								{
										pthread_create(&tid31[i],NULL,threadThree1,NULL);
								}

								for(i=0;i<8;i++)
								{
										pthread_create(&tid32[i],NULL,threadThree2,NULL);
								}
								break;
						case 4:								
								pthread_t tid41[48],tid42[48],tid43[2],tid44[2];
                                                                                                                        
								for(i=0;i<48;i++)
								{
										pthread_create(&tid41[i],NULL,threadFour1,NULL);
										pthread_create(&tid42[i],NULL,threadFour2,NULL);
								}

								pthread_create(&tid43[i],NULL,threadFour3,NULL);

								for(i=0;i<3;i++)
								{									
										pthread_create(&tid44[i],NULL,threadFour4,NULL);
								}

								break;
						default:
								printf("input error,please try again.\r\n");
								break;

				}
		}
		return 0;
}
