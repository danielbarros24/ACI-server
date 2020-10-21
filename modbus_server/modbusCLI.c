#include <stdint.h>
#include <stdio.h>

#define  N_REG 10 

int Write_multiple_regs (char*, int, int, int, uint16_t*);
int Read_h_regs (char* , int , int , int , uint16_t* );

int main(){
    uint16_t valores[N_REG];
    uint16_t novos_val[N_REG];

    valores[0] = 14;
    valores[1] = 15;
    valores[2] = 18;
    valores[3] = 18;
    valores[4] = 18;

    //Write_multiple_regs ("127.0.0.1", 502, 0, 1, valores);
    Read_h_regs ("127.0.0.1", 502, 0x1A, 5, novos_val);
    for(int i=0; i<5; i++) {
        printf("%d\n", novos_val[i]);
    }
}


