#include <stdio.h>
#include "cria_func.h"

typedef int (*func_ptr) (int x);
//int repassa(int);
int mult(int x, int y) {
  return x * y ;
}

unsigned char codigo[] = {0x55        //push   %rbp
  ,0x48 ,0x89 ,0xe5                   //mov    %rsp,%rbp
  ,0x48 ,0x83 ,0xec ,0x10             //sub    $0x10,%rsp
  ,0x4c ,0x89 ,0x65 ,0xf0             //mov    %r12,-0x10(%rbp)
  ,0x41 ,0x89 ,0xfc                   //mov    %edi,%r12d
  ,0x41 ,0xb8 ,0x0a ,0x00 ,0x00 ,0x00 //mov    $0xa,%r8d
  ,0x44 ,0x89 ,0xe7                   //mov    %r12d,%edi
  ,0x44 ,0x89 ,0xc6                   //mov    %r8d,%esi
  ,0x44 ,0x89 ,0xca                   //mov    %r9d,%edx
  ,0xe8 ,0x00 ,0x00 ,0x00 ,0x00       //callq  23 <repassa+0x23>
  ,0x4c ,0x8b ,0x65 ,0xf0             //mov    -0x10(%rbp),%r12
  ,0xc9                               //leaveq
  ,0xc3                               //retq
  };


int main (void) {

  DescParam params[2];
  func_ptr f_mult;
  

  params[0].tipo_val = INT_PAR; /* o primeiro parãmetro de mult é int */
  params[0].orig_val = PARAM;   /* a nova função repassa seu parämetro */


  params[1].tipo_val = INT_PAR; /* o segundo parâmetro de mult é int */
  params[1].orig_val = FIX;     /* a nova função passa para mult a constante 10 */
  params[1].valor.v_int = 10;

  // params[2].tipo_val = INT_PAR; /* o segundo parâmetro de mult é int */
  // params[2].orig_val = PARAM;     /* a nova função passa para mult a constante 10 */
  // params[2].valor.v_int = 11;
  unsigned long t= ((unsigned long)mult - (unsigned long)&codigo[35]);

  union
  {
    int i;
    char c[4];
  }u;
  
  u.i = (int) t;
  codigo[31] = u.c[0];
  codigo[32] = u.c[1];
  codigo[33] = u.c[2];
  codigo[34] = u.c[3];
  printf("codigo: ");
  int j;
  for(j=0; j<41; j++)
    {
        printf("%02X - ", codigo[j]);
    }

  printf("\nnewf: ");
  f_mult = (func_ptr) cria_func (mult, params, 2);
  //f_mult = (func_ptr) codigo;
  printf("\n");
  int i;
  for (i = 1; i <=10; i++) {
    printf("%d\n", f_mult(i)); /* a nova função só recebe um argumento */
  }

  //libera_func(f_mult);
  //printf("\n %p \n",f_mult);
  return 0;
}