#include "minunit.h"
#include <stdio.h>
#include "minunit.h"
#include "../src/list.h"

int tests_run = 0;

static char* test_listCreate() {
	PList list = listCreate();
	mu_assert("test_listCreate, count != 0", 0 == listSize(list));
	listFree(list);
	return 0;
}

static char* test_listPushFront() {
	PList list = listCreate();
	listPushFront(list, (void*)1);
	mu_assert("test_listPushBack, count != 1", 1 == listSize(list));
	listFree(list);
	return 0;
}

static char* test_listPopFrontEmpty() {
	PList list = listCreate();
	void* val = listPopFront(list);
	mu_assert("test_listPopFrontEmpty, val == NULL", NULL == val);
	mu_assert("test_listPopFrontEmpty, count != 0", 0 == listSize(list));
	listFree(list);
	return 0;
}

static char* test_listPopFront() {
	PList list = listCreate();
	listPushFront(list, (void*)1);
	void* val = listPopFront(list);
	mu_assert("test_listPopFrontEmpty, val == 1", 1 == (int)val);
	mu_assert("test_listPopFront, count != 0", 0 == listSize(list));
	listFree(list);
	return 0;
}

static char* test_listPushPopFront() {
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

static char* all_tests() {
	mu_run_test(test_listCreate);
	mu_run_test(test_listPushFront);
	mu_run_test(test_listPopFront);
	mu_run_test(test_listPopFrontEmpty);
	mu_run_test(test_listPushPopFront);
	return 0;
}

int main(int argc, char** argv) {
	char* result = all_tests();
	if (result != 0) {
		printf("%s\n", result);
	}
	else {
		printf("ALL TESTS PASSED\n");
	}
	printf("Tests run: %d\n", tests_run);

	return result != 0;
}
