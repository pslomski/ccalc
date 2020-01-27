/* file list.h*/

#ifndef __LIST_H__
#define __LIST_H__

typedef struct List* PList;

PList listCreate();
void listFree(PList this);
int listIsEmpty(PList this);
void listPushFront(PList this, void* val);
void* listPopFront(PList this);
void* listPeekFront(PList this);
int listSize(PList this);

#endif // __LIST_H__