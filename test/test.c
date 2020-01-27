#include <stdio.h>
#include "minunit.h"
#include "test_list.h"

int tests_run = 0;

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
