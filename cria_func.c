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


    /*auxiliar para ponteiro*/
    unsigned long var_ptr;

    //pushq %rbp //movq  %rsp, %rbp 
    unsigned char start[]={0x55, 0x48, 0x89, 0xe5};
    
    //leave //ret
    unsigned char end[]={0xc9, 0xc3};
    
    unsigned char *newf = (unsigned char *)malloc(sizeof(unsigned char)*200);

    //função movl                          
    //unsigned char movl[2] = {0x41, 0x89} ; 
    // unsigned char edi[3] = {0xfc, 0xf8, 0xf9} ; 
    // unsigned char esi[3] = {0xf4, 0xf0, 0xf1} ; 
    // unsigned char edx[3] = {0xd4, 0xd0, 0xd1} ;    
    unsigned char reg[][3] = {{0xfa, 0xf8, 0xf9}, {0xf2, 0xf0, 0xf1}, {0xd2, 0xd0, 0xd1}} ; 

    //                        %r10d  %r8d  %r9d
    unsigned char regSave[] = {0xba, 0xb8, 0xb9 }; 

    //                         %edi  %esi  %edx
    unsigned char regCall[] = {0xd7, 0xc6, 0xca}; 


    unsigned char saveD[] = {0x12, 0x00, 0x09}; 

    int shift;
    int i = 0;
    int j;
    int cont;
    int aux;
    //START
    for(j=0; j<4; j++)
    {
        newf[i] = start[j];
        i++;
    }
    //move os registradores dos paramentros para salva-los 
    //edi ->  r10d
    //esi ->  r8d
    //edx ->  r9d
    for( j = 0 ; j < n ; j++){
        
        if(params[j].orig_val == PARAM){
            //coloca o parametro no registrador certo para chamar a funcao
            newf[i] = 0x41;
            newf[i+1] = 0x89;
            newf[i+2] = reg[cont][j];
            i+=3;
            cont++;
        }
        else if (params[j].orig_val == FIX){
            newf[i] = 0x41;
            newf[i+1] = regSave[j];
            i+=2;
            u.i = params[j].valor.v_int;
            for(aux =0; aux < 4 ; aux++){
                newf[i] = u.c[aux];
                i++; 
            }
            
        } else if(params[j].orig_val == IND){

                newf[i] = 0x49;
				newf[i+1] = regSave[j];
				i+=2;

				var_ptr = (unsigned long) params[j].valor.v_ptr;
                for (aux = 0; aux<8; aux++){
                    newf[i] = (var_ptr >> aux*8) & 0xff;
                    i++;
                }

                newf[i] = 0x45;
                newf[i+1] = 0x8b;
                newf[i+2] = saveD[j];
                i+=3;
                
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
    u.i = (int) ((unsigned long)f - (unsigned long)&newf[i+5]);
    i++;
    for( j = 0 ; j < 4 ; j++){
        newf[i] = u.c[j];
        i++;
    }

    //END
    for(j=0; j<2; j++)
    {
        newf[i] = end[j];
        i++;
    }

    //Print
    for(j=0; j<27; j++)
    {
        printf("%02X - ", newf[j]);
    }
    
    return newf;
}