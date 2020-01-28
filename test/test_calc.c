#include <stdlib.h>
#include "minunit.h"
#include "../src/calc.h"

char* test_eval()
{
	mu_assert("test_calc, 0.0 == eval()", 0.0 == eval(""));
	mu_assert("test_calc, 0.0 == eval(0)", 0.0 == eval("0"));
	mu_assert("test_calc, 1.0 == eval(1)", 1.0 == eval("1"));
	return 0;
}