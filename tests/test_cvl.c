#include <stdio.h>

void test_correlate(void);
void test_imwrite(void);

int main(void) {
    printf("Running Unit Tests...\n");

    test_imwrite();
    test_correlate();

    printf("Passed\n");

    return 0;
}
