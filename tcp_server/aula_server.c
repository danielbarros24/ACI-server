#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 

#define MYPORT 22222
#define BUF_LEN 100

int main() {
    int so, sd, len;
    struct sockaddr_in loc, rem;
    socklen_t addlen = sizeof(loc);

    char buf[BUF_LEN];

    so = socket(PF_INET, SOCK_STREAM, 0);

    loc.sin_family = AF_INET;
    loc.sin_port = htons(MYPORT);
    inet_aton("127.0.0.1",&loc.sin_addr);

    bind(so, (struct sockaddr *) &loc, addlen);

    listen(so, 10);

    sd=accept(so, (struct sockaddr *) &rem, &addlen);

    while(1) {

        len=recv(sd, buf, BUF_LEN, 0);

    for (int i=0; i<len;i++) {
        if(buf[i] == '#'){
            break;
        }

        else{

            if ((buf[i] >= 'a')  && (buf[i] <= 'z')){

                buf[i] = buf[i] - 'a' + 'A';
            }
        }
    }

    len=send(sd, buf,len, 0);
    }
    

    close(so);

}
