#include <stdio.h>
#include "pgen.h"

int main()
{
	char expression[100];
	scanf("%100s", expression);
	_pgen_lex_expression(expression);
	return 0;
}