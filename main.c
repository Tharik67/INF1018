#include <stdio.h>
#include "cria_func.h"

typedef int (*func_ptr) (int x, int y);
//int repassa(int);
int mult(int x, int y) {
  return x * y;
}

int main (void) {

  DescParam params[2];
  func_ptr f_mult;
  

  params[0].tipo_val = INT_PAR; /* o primeiro parãmetro de mult é int */
  params[0].orig_val = PARAM;   /* a nova função repassa seu parämetro */

  params[1].tipo_val = INT_PAR; /* o segundo parâmetro de mult é int */
  params[1].orig_val = PARAM;     /* a nova função passa para mult a constante 10 */
  params[1].valor.v_int = 11;
  printf("%d\n", params[1].valor.v_int);
  f_mult = (func_ptr) cria_func (mult, params, 2);
  printf("\n");
  int i;
  for (i = 1; i <=10; i++) {
    printf("%d\n", f_mult(i, 11)); /* a nova função só recebe um argumento */
  }

  libera_func(f_mult);
  // printf("\n %p \n",f_mult);
  return 0;
}