#include "socket.h"





struct sockaddr_in servaddr;
struct sockaddr_in clientaddr;
int sock;
struct timeval timeOut;
void socketinit()
{
    if((sock = socket(PF_INET, SOCK_DGRAM, 0)) < 0){
        perror("socket");
        exit(EXIT_FAILURE);
    }

    timeOut.tv_sec = 0;                 //设置5s超时
    timeOut.tv_usec = 500;
    clientaddr.sin_family = AF_INET;
    clientaddr.sin_addr.s_addr = INADDR_ANY;
    clientaddr.sin_port = htons(9999);
    if (bind(sock, (struct sockaddr *) &clientaddr,sizeof(clientaddr)) < 0)
                   ;
    if (setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &timeOut, sizeof(timeOut)) < 0)
    {
            printf("time out setting failed\n");
    }
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(8080);
    servaddr.sin_addr.s_addr = inet_addr("10.0.2.15");
}

void senddata(char *sendbuf,int len)
{
    sendto(sock, sendbuf, len, 0, (struct sockaddr *)&servaddr, sizeof(servaddr));
}


int revdata(char *recvbuf,int len)
{
    return recvfrom(sock, recvbuf, len, 0, NULL, NULL);
}
