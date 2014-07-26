#include <stdio.h>

int total_test_cases = 0;
int failed_test_cases = 0;

int main(int argc, char **argv) {
  printf("Test cases: %d, passed %d, failed %d.\n", total_test_cases, total_test_cases - failed_test_cases, failed_test_cases);

  return failed_test_cases == 0 ? 0 : -1;
}
