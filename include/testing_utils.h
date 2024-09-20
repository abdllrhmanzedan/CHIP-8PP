#ifndef _TEST_UTIL_H
#define _TEST_UTIL_H
#include <string>

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

#define TEST_SUITE_START(suite_name) \
    printf("%sRUNNING TEST SUITE: %s%s\n", ANSI_COLOR_MAGENTA, suite_name, ANSI_COLOR_RESET)

#define TEST_SUITE_SUCCESS(suite_name) \
    printf("%sTEST SUITE PASSED! %s\n", ANSI_COLOR_GREEN, ANSI_COLOR_RESET)
    
#define TEST_PASS(test_name) printf("%-75s %s", test_name, ANSI_COLOR_GREEN " TEST PASSED!" ANSI_COLOR_RESET "\n")
#define TEST_FAIL(test_name) printf("%-75s %s", test_name, ANSI_COLOR_RED   " TEST FAILED!" ANSI_COLOR_RESET "\n")


void testTemplate(std::string test_name) {
    bool passed = true;

    if (passed)
        TEST_PASS(test_name);
    else
        TEST_FAIL(test_name);
}
void TEMPLATE_TEST_SUIT() {
    TEST_SUITE_START("X");

    TEST_SUITE_SUCCESS("X");
}


#endif