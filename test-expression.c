/*
 * @description   Test analýzy výrazù
 * @author        Vendula Poncová - xponco00
 * @projekt       IFJ11
 * @date
 */

#include <stdio.h>
#include <stdlib.h>
#include "table.h"
#include "str.h"
#include <string.h>
#include "stack.h"
#include "list.h"
#include "expression.h"


int main()
{
 tiskniPrecTab();
 lex = L_NUMBER; 
 //lex = L_RIGHT_BRACKET;
 parseExpression();
 return EXIT_SUCCESS;
}
