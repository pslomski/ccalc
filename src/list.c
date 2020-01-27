#include "list.h"
#include <stdlib.h>

typedef struct Node* PNode;

typedef struct Node{
	PNode next;
	void* val;
} Node;

PNode nodeCreate(void* val, PNode next)
{
	PNode node = malloc(sizeof(Node));
	if (node) {
		node->val = val;
		node->next = next;
	}
	return node;
}

typedef struct List{
	Node* front;
	int size;
} List;

PList listCreate()
{
	PList list = malloc(sizeof(List));
	if (list) {
		list->front = NULL;
		list->size = 0;
	}
	return list;
}

void listFree(PList this)
{
	free(this);
}

PNode listFront(PList this)
{
	return this->front;
}

int listIsEmpty(PList this)
{
	return this->front == NULL;
}

int listSize(PList this)
{
	return this->size;
}

void listPushFront(PList this, void* val)
{
	this->front = nodeCreate(val, this->front);
	this->size++;
}

void* listPopFront(PList this)
{
	void* val = NULL;
	if (this->front) {
		val = this->front->val;
		PNode tmp = this->front;
		this->front = this->front->next;
		free(tmp);
		this->size--;
	}
	return val;
}

void* listPeekFront(PList this)
{
	void* val = NULL;
	if (this->front) {
		val = this->front->val;
	}
	return val;
}