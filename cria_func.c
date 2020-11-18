#include "cria_func.h"
#include <stdio.h>
#include <stdlib.h>

void libera_func (void* func) {
    free(func);
}


void *cria_func(void *f,DescParam params[], int n)
{
    union {
        int i;
        char c[4];
    }u;
    
    //pushq %rbp //movq  %rsp, %rbp //sub $16, %rsp //mov %r12,-0x10(%rbp)
    unsigned char start[]={ 0x55, 0x48, 0x89, 0xe5, 0x48, 0x83, 0xec, 0x10, 0x4c, 0x89, 0x65, 0xf0};
    
    //mov -16(%rbp), %r12 //leave //ret
    unsigned char end[]={0x4c, 0x8b, 0x65, 0xf0, 0xc9, 0xc3 };
    
    unsigned char *newf = (unsigned char *)malloc(sizeof(unsigned char)*200);
    
    unsigned char movl[][3] = {{0x41, 0x89, 0xfc}, {0x41, 0x89, 0xf0}, {0x41, 0x89, 0xd1}} ; 
    unsigned char regSave[] = {0xb8, 0xb9, 0xbc}; 
    unsigned char regCall[] = {0xe7, 0xc6, 0xca}; 

    int i;
    int j;
    int cont;
    //START
    for(j=0; j<12; j++)
    {
        newf[i] = start[j];
        i++;
    }
    //move os registradores dos paramentros para salva-los 
    //edi ->  r12d
    //esi ->  r8d
    //edx ->  r9d
    for( j = 0 ; j < n ; j++){
        if(params[j].orig_val == PARAM){
            for(cont =0; cont < 3 ; cont++){
                newf[i] = movl[j][cont];
                i++;
            }
        }else if (params[j].orig_val == FIX)
        {
            newf[i] = 0x41;
            newf[i+1] = regSave[j];
            i+=2;
            u.i = params[j].valor.v_int;
            for(cont =0; cont < 4 ; cont++){
                newf[i] = u.c[cont];
                i++;
            }

        }
        
    }
    //move para os registradores de parametros para chamar a funcao;
    //r12-> rdi
    //r8 -> rsi  
    //r9 -> rdx 
    for( j = 0 ; j < n ; j++){
        newf[i] = 0x44 ;
        newf[i+1] = 0x89 ;
        i+=2;
        newf[i] = regCall[j];
        i++;
    }    
    //chama a funcao f 
    newf[i] = 0xe8; 
    i++;
    u.i = (int) ((unsigned long)f - (unsigned long)&newf[i+5]);
    for( j = 0 ; j < 4 ; j++){
        newf[i] = u.c[j];
        i++;
    }

    //END
    for(j=0; j<6; j++)
    {
        newf[i] = end[j];
        i++;
    }
    //Print
    for(j=0; j<45; j++)
    {
        printf("%02X - ", newf[j]);
    }
    
    return f;
}