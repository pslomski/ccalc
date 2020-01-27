#include <stdlib.h>
#include "minunit.h"
#include "../src/list.h"

char* test_listCreate() {
	PList list = listCreate();
	mu_assert("test_listCreate, count != 0", 0 == listSize(list));
	listFree(list);
	return 0;
}

char* test_listPushFront() {
	PList list = listCreate();
	listPushFront(list, (void*)1);
	mu_assert("test_listPushBack, count != 1", 1 == listSize(list));
	listFree(list);
	return 0;
}

char* test_listPopFrontEmpty() {
	PList list = listCreate();
	void* val = listPopFront(list);
	mu_assert("test_listPopFrontEmpty, val == NULL", NULL == val);
	mu_assert("test_listPopFrontEmpty, count != 0", 0 == listSize(list));
	listFree(list);
	return 0;
}

char* test_listPopFront() {
	PList list = listCreate();
	listPushFront(list, (void*)1);
	void* val = listPopFront(list);
	mu_assert("test_listPopFrontEmpty, val == 1", 1 == (int)val);
	mu_assert("test_listPopFront, count != 0", 0 == listSize(list));
	listFree(list);
	return 0;
}

char* test_listPushPopFront() {
	PList list = listCreate();
	for (int i = 0; i < 10; i++) {
		listPushFront(list, (void*)i);
		mu_assert("test_listPushPopFront, count", (i + 1) == listSize(list));
	}
	for (int i = 0; i < 10; i++) {
		void* val = listPopFront(list);
		mu_assert("test_listPushPopFront, count", (10 - i - 1) == listSize(list));
		mu_assert("test_listPushPopFront, val", (10 - i - 1) == (int)val);
	}
	void* val = listPopFront(list);
	listFree(list);
	return 0;
}