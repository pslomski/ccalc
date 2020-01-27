#include "stack.h"
#include "list.h"
#include <stdlib.h>
#include <stdio.h>

static error(const char* message)
{
	printf(message);
	exit(1);
}

typedef struct Stack {
	PList list;
} Stack;

PStack stackCreate()
{
	PStack stack = malloc(sizeof(Stack));
	if (stack)
		stack->list = listCreate();
}

void stackFree(PStack this)
{
	free(this->list);
	free(this);
}

void stackPush(PStack this, void* val)
{
	listPushFront(this->list, val);
}

void* stackPop(PStack this)
{
	if (stackSize(this) <= 0)
		error("stackPop: stack underflow");
	return listPopFront(this->list);
}

void* stackPeek(PStack this)
{
	void* val = listPeekFront(this->list);
}

int stackSize(PStack this)
{
	return listSize(this->list);
}
