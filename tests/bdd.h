#ifndef __TEST_BDD_FRAMEWORK_H__
#define __TEST_BDD_FRAMEWORK_H__

#include <stdbool.h>
#include <stdio.h>

#define CONCAT(x, y) x##y
#define CONCAT1(x, y) CONCAT(x, y)

#define Feature(x) \
  __attribute__ ((__constructor__)) static void __bdd_test_feature__(void) { \
    printf("Feature: %s", x); 

#define Case(x) } \
  __attribute__ ((__constructor__)) static void CONCAT1(__bdd_test_case, __LINE__) (void) { \
  total_test_cases++; \
  bool __bdd_test_case_failed = false;\
  __BDD_TEST_OUTPUT("\n  Scenario:", x);

#define End \
    printf("\n"); \
  }

#define __BDD_TEST_OUTPUT(x, y) \
  printf("%s %s\n", x, y);

#define assert(x) \
  if (x) { \
    printf("      ...Passed\n"); \
  } \
  else { \
    if (!__bdd_test_case_failed) { \
      failed_test_cases++; \
      __bdd_test_case_failed = true; \
    } \
    printf("      ...Failed\n"); \
  }

#define Given(x) __BDD_TEST_OUTPUT("    Given", x);
#define When(x) __BDD_TEST_OUTPUT("    When", x);
#define Then(x) __BDD_TEST_OUTPUT("    Then", x);
#define And(x) __BDD_TEST_OUTPUT("    And", x);

extern unsigned int total_test_cases;
extern unsigned int failed_test_cases;

#endif
