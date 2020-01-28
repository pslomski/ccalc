/* file stack.h*/

#ifndef __STACK_H__
#define __STACK_H__

typedef struct Stack* PStack;

PStack stackCreate();
void stackFree(PStack this);
void stackPush(PStack this, void* val);
void* stackPop(PStack this);
void* stackPeek(PStack this);
int stackSize(PStack this);
int stackIsEmpty(PStack this);

#endif // __STACK_H__