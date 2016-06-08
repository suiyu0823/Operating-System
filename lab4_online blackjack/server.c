//Yu Sui 500412700
#include "dealer.h"

#define PORT 2700

int message_confirm(int sock, int timeout)
{
    char buffer[32];
    int ret = sock_recv(sock, buffer, sizeof(buffer) - 1, timeout);
    if (0 == strcmp("0xdeadbeef", buffer))
    {
         printf("player confirmed!\n");
         buffer[0]='\0';
         return 1;
    }
    else
    {
        puts("player input timeout, kick it out!");
        return 0;
    }
}

int accept_socket(int listen_socket, int timeout)
{
    struct sockaddr_storage addr;
    socklen_t len = sizeof(struct sockaddr_storage);
    int sock = select_fd(listen_socket, timeout);
    if (sock > 0) {
        return accept(listen_socket, (struct sockaddr*)&addr, &len);
    }
    return -1;
}

void game_loop(int listen_socket)
{
    time_t start_time = 0;
    time_t current_time = 0;
    players.top=-1;
    pid_t dealer_pid;
    
    while (1) {
        int accept_sock = accept_socket(listen_socket, 0.1);
        if (accept_sock > 0) {
            int valid = message_confirm(accept_sock,5);
            if (valid) {
                if (start_time == 0)
                {
                    start_time = time(NULL);
                    puts("first player join...\n");
                }
                players.fds[++players.top] = accept_sock;
            } else {
                close(accept_sock);
            }
        }
        
        current_time = time(NULL);
        
        if (start_time > 0 && (current_time - start_time > 30 ||players.top >= 3)) {
            printf("\ntime used=%ds \nplayer_number=%d\n",(int)(current_time-start_time),players.top+1);
            
            dealer_start(players);
            start_time = 0;
            
            while (players.top >0) {
                int fd =players.fds[players.top--];
                close(fd);
            }
          
          printf("\ncurrent game round is over, wait for the next round...\n");
          players.top=-1;
        }
    }
}

int main()
{
    int server_socket;
    struct sockaddr_in server_addr,client_addr;
    
    server_socket=socket(AF_INET,SOCK_STREAM,0);
    
    printf("server is ready...\n!");
    
    bzero(&server_addr,sizeof(struct sockaddr_in));   
    server_addr.sin_family=AF_INET;
    server_addr.sin_addr.s_addr=htonl(INADDR_ANY);
    server_addr.sin_port=htons(PORT);  
    bind(server_socket,(struct sockaddr*)(&server_addr),sizeof(struct sockaddr));
    listen(server_socket,10);  
    
    game_loop(server_socket);
}

