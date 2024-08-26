// include/engine.h

#ifndef ENGINE_H
#define ENGINE_H

typedef struct Value {
    float data;
    float grad;
    void (*backward)(struct Value*);
    struct Value* prev[2];
    char op[5];
} Value;

Value create_value(float data);
void free_value(Value* v);
void add_backward(Value* v);
Value add(Value* a, Value* b);
void prod_backward(Value* v);
Value prod(Value* a, Value* b);
void pow_backward(Value* v);
Value power(Value* a, Value* power_val);
void exp_backward(Value* v);
Value exponent(Value* a);
void tanh_backward(Value* v);
Value tan_h(Value* a);
void relu_backward(Value* v);
Value relu(Value* a);
Value divide(Value* a, Value* b);
void print_val(Value* v);

#endif
