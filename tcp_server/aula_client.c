#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <netdb.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 

#define MYPORT 22222
#define BUF_LEN 100


int main () {

    int sock, len;
    struct sockaddr_in serv;

    socklen_t addlen = sizeof(serv);
    char buf[BUF_LEN];

    sock = socket(PF_INET, SOCK_STREAM, 0);

    serv.sin_family = AF_INET;
    serv.sin_port = htons(MYPORT);
    inet_aton("127.0.0.1",&serv.sin_addr);

    connect(sock, (struct sockaddr *) &serv, addlen);

    while(1){

        scanf("%s", buf);

        len=send(sock, buf, strlen(buf)+1, 0);

        len=recv(sock, buf, len, 0);

        printf("%s\n", buf);

    }
    

    close(sock);

}
