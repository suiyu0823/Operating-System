//Yu Sui 500412700
#include "dealer.h"

void sig_usr(int signo)
{
	sigflag=1;
}

void tell_wait()
{
	signal(SIGUSR1,sig_usr);
	signal(SIGUSR2,sig_usr);
	sigemptyset(&zeromask);
	sigemptyset(&newmask);
	sigaddset(&newmask,SIGUSR1);
	sigaddset(&newmask,SIGUSR2);
	sigprocmask(SIG_BLOCK,&newmask,&oldmask);
}

void tell_parent(pid_t pid)
{
	kill(pid,SIGUSR2);
}


void wait_parent()
{
	while(sigflag==0)
		sigsuspend(&zeromask);
	sigflag=0;
	
	sigprocmask(SIG_SETMASK,&oldmask,NULL);
}


void tell_child(pid_t pid)
{
	kill(pid,SIGUSR1);
}

void wait_child()
{
	while(sigflag==0)
		sigsuspend(&zeromask);
	sigflag=0;
	
	sigprocmask(SIG_SETMASK,&oldmask,NULL);
}

int select_fd(int fd, int maxtime)
{
     fd_set fdset;
     struct timeval timeout;
     int ret;

     FD_ZERO(&fdset);
     FD_SET(fd, &fdset);
 

     timeout.tv_sec  = maxtime;
     timeout.tv_usec = 0;

     do {
          ret = select(fd + 1, &fdset, NULL, NULL, &timeout);
     } while (ret < 0 && errno == EINTR);

     return ret;
}

int sock_recv(int fd, void *buf, size_t bufsize, int timeout)
{
     int ret;

     if (timeout > 0) {
          ret = select_fd(fd, timeout);
          if (ret == 0)
               errno = ETIMEDOUT;
          if (ret <= 0)
               return -1;
     }

     do {
          ret = recv(fd, buf, bufsize,0);
     } while (ret == -1 && errno == EINTR);

     return ret;
}


int get_value(char character)
{
    switch(character)
    {
        case 'A': return 1;
        case '2': return 2;
        case '3': return 3;
        case '4': return 4;
        case '5': return 5;
        case '6': return 6;
        case '7': return 7;
        case '8': return 8;
        case '9': return 9;
        case 'T': return 10;
        case 'J': return 10;
        case 'Q': return 10;
        case 'K': return 10;
        default: return -1;
    }
}

char character_rand(int rand)
{
    switch(rand)
    {
        case 1: return 'A';
        case 2: return '2';
        case 3: return '3';
        case 4: return '4';
        case 5: return '5';
        case 6: return '6';
        case 7: return '7';
        case 8: return '8';
        case 9: return '9';
        case 10: return 'T';
        case 11: return 'J';
        case 12: return 'Q';
        case 13: return 'K';
        default: return 0;
    }
}

char suit_rand(int rand)
{
    switch(rand)
    {
        case 1: return 'S';
        case 2: return 'H';
        case 3: return 'D';
        case 4: return 'C';
        default: return 0;
    }
}

int generate_card(char* card,int seed)
{
    srand(time(NULL)+seed);     
    card[0]=character_rand(rand()%13+1);
    sleep(1);    
    card[1]=suit_rand(rand()%4+1);
    card[2]='\0';
    return get_value(card[0]);
}

void dealer_hand(int socket)
{ 
    char buffer[32];
    char player_card[3];
    int playerstat;
    char dbuffer;
    int input;
    int playerscore=0,dealerscore=0;
    char dealercards[30];
  //  int playerscore=0;
  //  int dealerscore
    int connect_socket=socket;
    long process_id=getpid();
    int seed=process_id;   
    printf("process %ld *** socket %d\n",process_id,connect_socket);
    
    dealerscore=generate_card(dealer_card,0);
    strcat(dealercards,dealer_card);
   
    playerscore+=generate_card(buffer,seed); 
    playerscore+=generate_card(player_card,seed);
    strcat(buffer,player_card); 
    strcat(buffer,dealer_card);
    write(connect_socket,buffer,7);    
    printf("socket %d: send %s\n",connect_socket,buffer);
    sleep(1);
    bzero(buffer,32);
    
    while(1)
    {
        sock_recv(connect_socket,buffer,32,10); 
        
    	if(0==strcmp(buffer,"HIT"))
    	{
       	 	printf("socket %d HIT\n",connect_socket);
        
        	playerscore+=generate_card(buffer,seed);
        	write(connect_socket,buffer,3);
        	printf("socket %d: send %s\n",connect_socket,buffer);
        	if(playerscore>21)
        	{
            	printf("socket %d: go burst\n",connect_socket);
            	playerstat=BURST;
            	break;
        	}
        	sleep(1);
   	 	}
    	else
    	{
    		break;
    	}
    }
    if(0==strcmp(buffer,"STAND"))
    {
        printf("socket %d: STAND\n",connect_socket);
        printf("socket %d: wait dealer to play with me!\n",connect_socket);
    	playerstat=STAND;
    }
    else
    {
        if(0!=strcmp(buffer,"HIT"))
    	{ 
    	   printf("input timeout, kick out the player!\n");
    	   playerstat=INVALID;
    	}
    }  
    
    tell_parent(getppid());
    printf("process %ld send signal to parent!\n",process_id);
    wait_parent();
    
     if(playerstat==STAND)
        {
            printf("play with socket %d...\n",connect_socket);
            while(1)
            { 
                printf("dealer's cards : %s(%d) ,player's score: %d\n",dealercards,dealerscore,playerscore);
                printf("input y to send HIT, or n to send STAND!\n");
                fflush(stdin);     
                while((input=getchar())=='\n'||input=='\0');
                if(input=='y')
                {
                    printf("HIT\n");
                    dealerscore+=generate_card(dealer_card,connect_socket); 
                    strcat(dealercards,dealer_card);
                    printf("get card %s\n",dealer_card);
                    write(connect_socket,dealer_card,3);
                   
                    if(dealerscore>21)
                    {   
                        printf("dealer has gone burst! client win\n");
                        close(connect_socket);
                        break;
                    }
                    
                    read(connect_socket,dbuffer,32);
                }
                else if(input=='n')
                {
                    printf("STAND\n");
                    write(connect_socket,"*",2);
                    if(dealerscore>playerscore)
                        printf("dealer win!\n");
                    else if(dealerscore<playerscore)  
                        printf("client win!\n");
                    else
                        printf("draw game!\n");
                    close(connect_socket);
                    break;
                }
                else
                {
                    printf("invalid input, input again!\n");
                }
            }
        }
        else
        {
        	close(connect_socket);
        }
        printf("process %ld exit!\n",process_id);
    // exit(0);
}


void dealer_start(Players players)
{
	int i;
    int player_number=players.top+1;
    int cpid[player_number];
    pid_t pid;
       
    tell_wait();
    
    for(i=0;i<player_number;i++)
    {
    	pid=fork();
    	if(pid==0)
    	{
    		dealer_hand(players.fds[i]);
    		exit(0);
    	}
    	else
    	{
    		cpid[i]=pid;
    	}
    }
    for(i=0;i<player_number;i++)
    {
    	wait_child();
    }
    printf("it's dealer's turn to player with stand players!\n");

    for(i=0;i<player_number;i++)
    {
        tell_child(cpid[i]);
        waitpid(cpid[i],NULL,0);
    }
}


