#include <stdlib.h>   
#include <sys/types.h>   
#include <stdio.h>   
#include <sys/socket.h>   
#include <linux/in.h>   
#include <string.h>
#include <errno.h>    
#include <signal.h>

typedef struct playerstack {
    int fds[4];
    int top;
}Players;

Players players;

enum {
    INVALID=-1,
    BURST= 0,
    STAND=1,
};

int sigflag;
static sigset_t newmask,oldmask,zeromask;

char dealer_card[3];

void sig_usr(int signo);
void tell_wait();
void tell_parent(pid_t pid);
void tell_child(pid_t pid);
void wait_parent();
void wait_child();

int select_fd(int fd, int maxtime);
int sock_recv(int fd, void *buf, size_t bufsize, int timeout);

int get_value(char ch_card_value);
char character_rand(int rand);
char suit_rand(int rand);
int generate_card(char* card,int seed);
void dealer_hand(int socket);
void dealer_start(Players players);
