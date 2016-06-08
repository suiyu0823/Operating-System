//Yu Sui 500412700
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>
#include <signal.h>  
#include <sys/types.h>   
#include <sys/socket.h>   
#include <linux/in.h>   


#define IP "10.0.2.15"
#define PORT 2700

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

int main(int argc, char **argv)
{
   
    char handcard[30];
    char dealercard[30];
    int clientscore=0;
    int dealerscore=0;
    int inputValue;
    int clientsocket;
    char buffer_zone[32];


    struct sockaddr_in server_addr;
   
    bzero(&server_addr,sizeof(struct sockaddr_in));   
    server_addr.sin_family=AF_INET;
    server_addr.sin_addr.s_addr=inet_addr(IP);
    server_addr.sin_port=htons(PORT);

    socklen_t server_addr_length = sizeof(server_addr);

    clientsocket=socket(AF_INET,SOCK_STREAM,0);
    printf("client is ready to connect...\n");

    
    if(connect(clientsocket,(struct sockaddr*)&server_addr, server_addr_length) > 0)  
    {  
        printf("connect failure!\n");  

        return -1;  
    } 
    else
    {
        printf("successfully connect to the server!\n");  
    }
    
    printf("input y to send 0xdeadbeef in 5s, or you will be kicked out!\n");
    if(getchar()=='y')
    {
        send(clientsocket,"0xdeadbeef",11,0);
    } 
    else
    {
        close(clientsocket);
    }
    recv(clientsocket,buffer_zone,32,0);
    


    handcard[0]=buffer_zone[0];
    handcard[1]=buffer_zone[1];
    handcard[2]=buffer_zone[2];
    handcard[3]=buffer_zone[3];
    handcard[4]='\0';
    dealercard[0]=buffer_zone[4];
    dealercard[1]=buffer_zone[5];
    dealercard[2]='\0';
  

    
    printf("get cards:%s\n",buffer_zone);
    
    clientscore=get_value(handcard[0])+get_value(handcard[2]);
    dealerscore=get_value(dealercard[0]);
    
    printf("My cards: %s(%d)\n Dealer's cards: %s(%d)\n",handcard,clientscore,dealercard,dealerscore);
    
    while(2>1)
    {
        printf("input 'y' to HIT,  'n' to STAND in 10s! or you will be kicked out!\n");
        fflush(stdin);   

        while((inputValue=getchar())=='\n'||inputValue=='\0');
        
        if(inputValue=='y')
        {
            send(clientsocket,"HIT",4,0);
            printf("HIT!\n");
            bzero(buffer_zone,32);
            recv(clientsocket,buffer_zone,32,0);
            printf("get card:%s\n",buffer_zone);
            strcat(handcard,buffer_zone);
            clientscore+=get_value(buffer_zone[0]);
            printf("My cards: %s(%d) Dealer's cards: %s(%d)\n\n\n",handcard,clientscore,dealercard,dealerscore);
            
            if(clientscore>21)
            {
                printf("Burst! Game Over!\n");
                close(clientsocket);
                return;
            }
        }
        else if(inputValue=='n')
        {
             printf("STAND!\n");
            send(clientsocket,"STAND",6,0);
            printf("send stand!\n");
            break;
        }
        else
        {
            close(clientsocket);
            return;
        }  
    }
    printf("Dealer is playing with you now, Waiting for card...\n");
    


    while(1)
    {
        read(clientsocket,buffer_zone,32); 
        if(0==strcmp("*",buffer_zone))    // receive "#" means dealer standed
        {
            printf("Dealer stand! Your score is %d, and dealer's score is %d\n",clientscore,dealerscore);
            if(dealerscore>clientscore)
                printf("You lose!\nYou are going to quit!\n");
            else if(dealerscore<clientscore)  
                printf("You win!\nYou are going to quit!\n");
            else
                printf("draw game!\nYou are going to quit!\n");
            close(clientsocket);
            break;
        }
        printf("get dealer's card:%s\n",buffer_zone);
        strcat(dealercard,buffer_zone); 
        dealerscore+=get_value(buffer_zone[0]);
        printf("my card: %s(%d) dealer's card: %s(%d)\n\n",handcard,clientscore,dealercard,dealerscore);
        if(dealerscore>21)
        {
            printf("Dealer burst, You win!\nYou are going to quit!\n");
            close(clientsocket);
            break;
        }
        write(clientsocket,"0",2);  
    }  
}

