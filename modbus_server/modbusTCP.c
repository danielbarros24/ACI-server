#include <stdint.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <netdb.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 

#define UNIT_ID 1

int Send_Modbus_request (char* server_add, int port, uint8_t* APDU,int APDUlen, uint8_t* APDU_R)
{   

    int sock, len;
    struct sockaddr_in serv;
    socklen_t addlen = sizeof(serv);
    static int TI=0;
    uint8_t MBAP[255];
    // generates TI (trans.ID →sequence number)
    TI++;

    // assembles PDU = APDU(SDU) + MBAP
    MBAP[0] = (uint8_t)((TI>>8) & 0xFF); 
    MBAP[1] = (uint8_t) (TI & 0xFF);
    MBAP[2] = 0;
    MBAP[3] = 0;
    len = 1 + APDUlen;
    MBAP[4] = (uint8_t)((len>>8) & 0xFF); 
    MBAP[5] = (uint8_t) (len & 0xFF);
    MBAP[6] = UNIT_ID;
    // opens TCP client socket and connects to server (*)

    sock = socket(PF_INET, SOCK_STREAM, 0);

    serv.sin_family = AF_INET;
    serv.sin_port = htons(port);
    inet_aton(server_add,&serv.sin_addr);

    connect(sock, (struct sockaddr *) &serv, addlen);

    
    write (sock, MBAP, 7);      // sends Modbus TCP PDU
    write (sock, APDU, APDUlen);

    //espera por resposta do servidor
    read (sock, MBAP, 7); //primeiro ler os 7 bytes do cabeçalho pois não sabemos o comprimento  do APDU   // response o timeout
    len = (int)((MBAP[4]<<8) + MBAP[5]);
    read (sock, APDU_R, len-1);
    
    // if response, remove MBAP, PDU_R → APDU_R

    
    // closes TCP client socket with server (*)
    close (sock);

    // returns: APDU_R and 0 – ok, <0 – error (timeout)
    return(0);
}

