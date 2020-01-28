#include <stdlib.h>
#include "minunit.h"
#include "../src/stack.h"

char* test_stackCreate()
{
	PStack stack = stackCreate();
	mu_assert("test_stackCreate, 0 == stackSize", 0 == stackSize(stack));
	stackFree(stack);
	return 0;
}

char* test_stackIsEmpty()
{
	PStack stack = stackCreate();
	mu_assert("test_stackCreate, 1 == stackIsEmpty", 1 == stackIsEmpty(stack));
	stackPush(stack, (void*)1);
	mu_assert("test_stackCreate, 0 == stackIsEmpty", 0 == stackIsEmpty(stack));
	stackPop(stack);
	mu_assert("test_stackCreate, 1 == stackIsEmpty", 1 == stackIsEmpty(stack));
	stackFree(stack);
	return 0;
}

char* test_stackPushPop()
{
	PStack stack = stackCreate();
	stackPush(stack, (void*)1);
	mu_assert("test_stackPush, 1 == stackSize(stack)", 1 == stackSize(stack));
	stackPush(stack, (void*)1);
	mu_assert("test_stackPush, 2 == stackSize(stack)", 2 == stackSize(stack));
	stackPop(stack);
	mu_assert("test_stackPush, 1 == stackSize(stack)", 1 == stackSize(stack));
	stackPop(stack);
	mu_assert("test_stackPush, 0 == stackSize(stack)", 0 == stackSize(stack));
	stackFree(stack);
	return 0;
}