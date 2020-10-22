#include <stdint.h>
#include <stddef.h>
#include <stdio.h>

#define  N_REG 10
#define ERR_BAD_nr -5
#define ERR_SEND -6 
#define error_code -7
#define ERR_MODBUS -9

int Send_Modbus_request(char*, int, uint8_t* , int, uint8_t*);



int Write_multiple_regs (char* server_add, int port , int st_r, int n_r, uint16_t* val) {

    uint8_t APDU[253], APDU_R[5];
 
    // check consistency of parameters
    if((n_r < 1) || (n_r > 123)) {
        return (ERR_BAD_nr);
    }

    // assembles APDU (MODBUS PDU)
    APDU[0] = 0x10;
    APDU[1] = (uint8_t)((st_r>>8) & 0xFF); 
    APDU[2] = (uint8_t) (st_r & 0xFF);
    APDU[3] = (uint8_t)((n_r>>8) & 0xFF);
    APDU[4] = (uint8_t) (n_r & 0xFF);
    APDU[5] = (uint8_t) (2*n_r);
    
    for(int i=0;i<n_r;i++) {
        APDU[6+2*i] = (uint8_t)((val[i]>>8) & 0xFF);
        APDU[6+2*i+1] = (uint8_t)(val[i] & 0xFF);
    }

    int APDUlen = 1 + 2 + 2 + 1 + 2*n_r; //bytes que compoe o APDU

    if(Send_Modbus_request(server_add, port, APDU, APDUlen, APDU_R) < 0)
        return(ERR_SEND);

    
    // checks the reponse (APDU_R or error_code)
    for(int j=0;j<5;j++) {

        if(APDU_R[j] < 0) {

            return(error_code);
    }
    }
    // returns: number of written registers – ok, <0 – error
    return n_r;

}

int Read_h_regs (char* server_add, int port, int st_r, int n_r, uint16_t* val)
{
    uint8_t APDU[253], APDU_R[253];
    // check consistency of parameters
    if((n_r < 1) || (n_r>123)) {
        return(ERR_BAD_nr);
    }
    // assembles APDU (MODBUS PDU)
    APDU[0] = 0x03;
    APDU[1] = (uint8_t)((st_r >> 8) & 0xFF);
    APDU[2] = (uint8_t)(st_r & 0xFF);
    APDU[3] = (uint8_t)((n_r >> 8) & 0xFF);
    APDU[4] = (uint8_t)(n_r & 0xFF);
  
    int APDUlen = 1 + 2 + 2; //bytes que compem o APDU
    
    if(Send_Modbus_request (server_add,port,APDU,APDUlen,APDU_R) < 0)
        return(ERR_SEND);
    

    // checks the reponse (APDU_R or error_code)
    uint8_t aux = (uint8_t)(APDU_R[0]);

    if (aux < 0) {
        return(error_code);
    }
    int aux1 = 0;

    for(int j=2;j<n_r*2+2;j+=2) {
        val[aux1]=(uint16_t)((APDU_R[j]<<8) | APDU_R[j+1]);
        aux1++;
    }

    if(APDU_R[0] > 0x80) return ERR_MODBUS;

    // returns: number of read registers – ok, <0 – error
    return n_r;

}
