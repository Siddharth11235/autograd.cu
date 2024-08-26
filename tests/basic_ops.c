// tests/basic_ops.c

#include "../include/engine.h"
#include <stdio.h>
#include <assert.h>

void test_add() {
    Value a = create_value(2.0);
    Value b = create_value(3.0);
    Value c = add(&a, &b);

    assert(c.data == 5.0);
    printf("test_add passed\n");
}

void test_prod() {
    Value a = create_value(2.0);
    Value b = create_value(3.0);
    Value c = prod(&a, &b);

    assert(c.data == 6.0);
    printf("test_prod passed\n");
}

void test_divide() {
    Value a = create_value(6.0);
    Value b = create_value(3.0);
    Value c = divide(&a, &b);

    assert(c.data == 2.0);
    printf("test_divide passed\n");
}

void test_backward_add() {
    Value a = create_value(2.0);
    Value b = create_value(3.0);
    Value c = add(&a, &b);

    c.grad = 1.0;
    c.backward(&c);

    assert(a.grad == 1.0);
    assert(b.grad == 1.0);
    printf("test_backward_add passed\n");
}

void test_backward_prod() {
    Value a = create_value(2.0);
    Value b = create_value(3.0);
    Value c = prod(&a, &b);

    c.grad = 1.0;
    c.backward(&c);

    assert(a.grad == 3.0);
    assert(b.grad == 2.0);
    printf("test_backward_prod passed\n");
}

int main() {
    test_add();
    test_prod();
    test_divide();
    test_backward_add();
    test_backward_prod();

    printf("All tests passed!\n");
    return 0;
}
