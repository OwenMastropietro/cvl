#include <stdio.h>

void test_imread(void);
void test_imwrite(void);
void test_correlate(void);

int main(void) {
    printf("Running Unit Tests...\n");

    test_imwrite();
    test_imread();
    test_correlate();

    printf("Passed\n");

    return 0;
}
