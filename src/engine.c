#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/engine.h"


Value create_value(float data) {
  Value v;
  v.data = data;
  v.grad = 0;
  v.backward = NULL;
  v.prev[0] = v.prev[1] = NULL;
  v.op[0] = '\0';
  return v;
}

void free_value(Value *v) { free(v); }

void add_backward(Value *v) {
  v->prev[0]->grad += v->grad;
  v->prev[1]->grad += v->grad;
  if (v->prev[0]->backward)
    v->prev[0]->backward(v->prev[0]);
  if (v->prev[1]->backward)
    v->prev[1]->backward(v->prev[1]);
}

Value add(Value *a, Value *b) {

  Value out = create_value(a->data + b->data);

  out.op[0] = '+';
  out.op[1] = '\0';
  out.prev[0] = a;
  out.prev[1] = b;
  out.backward = &add_backward;
  return out;
}

void prod_backward(Value *v) {
  v->prev[0]->grad += v->grad * v->prev[1]->data;
  v->prev[1]->grad += v->grad * v->prev[0]->data;
  if (v->prev[0]->backward)
    v->prev[0]->backward(v->prev[0]);
  if (v->prev[1]->backward)
    v->prev[1]->backward(v->prev[1]);
}
Value prod(Value *a, Value *b) {
  Value out = create_value(a->data * b->data);
  out.op[0] = '*';
  out.op[1] = '\0';
  out.prev[0] = a;
  out.prev[1] = b;
  out.backward = &prod_backward;

  return out;
}

void pow_backward(Value *v) {

  float power = v->prev[1]->data;
  v->prev[0]->grad += v->grad * (power * pow(v->prev[0]->data, (power - 1)));

  if (v->prev[0]->backward)
    v->prev[0]->backward(v->prev[0]);
}

Value power(Value *a, Value *power_val) {
  Value out = create_value(pow(a->data, power_val->data));
  out.op[0] = '^';
  out.op[1] = '\0';
  out.prev[0] = a;
  out.prev[1] = power_val;
  out.backward = &pow_backward;

  return out;
}

void exp_backward(Value *v) {
  v->prev[0]->grad += v->grad;
  if (v->prev[0]->backward)
    v->prev[0]->backward(v->prev[0]);
}

Value exponent(Value *a) {
  Value out = create_value(exp(a->data));
  snprintf(out.op, sizeof(out.op), "exp");
  out.prev[0] = a;
  out.backward = &exp_backward;
  return out;
}

void tanh_backward(Value *v) {
  v->prev[0]->grad += v->grad * (1 - pow(v->data, 2));
  if (v->prev[0]->backward)
    v->prev[0]->backward(v->prev[0]);
}

Value tan_h(Value *a) {
  Value out = create_value(tanh(a->data));
  snprintf(out.op, sizeof(out.op), "tanh");
  out.prev[0] = a;
  out.backward = &tanh_backward;
  return out;
}

void relu_backward(Value *v) {
  float val = v->data;
  float prev_grad = 0;

  if (val > 0) {
    prev_grad = 1;
  }
  v->prev[0]->grad += v->grad * prev_grad;
  if (v->prev[0]->backward)
    v->prev[0]->backward(v->prev[0]);
}

Value relu(Value *a) {
  float val = 0;
  if (a->data > 0) {
    val = a->data;
  }

  Value out = create_value(val);
  snprintf(out.op, sizeof(out.op), "relu");
  out.prev[0] = a;
  out.backward = &relu_backward;
  return out;
}

Value divide(Value *a, Value *b) {
  Value *inverse = (Value *)malloc(sizeof(Value));
  *inverse = create_value(-1);

  Value *recip_b = (Value *)malloc(sizeof(Value));
  *recip_b = power(b, inverse);

  Value out = prod(a, recip_b);
  return out;
}

void print_val(Value *v) { printf("Data: %f, Grad: %f\n", v->data, v->grad); }

int main() {
  Value a = create_value(2);
  Value b = create_value(1);
  Value f = create_value(2);

  Value x = add(&b, &f);
  Value power_val = create_value(3);

  Value c = divide(&a, &x);
  Value d = create_value(6.0);

  Value e = prod(&c, &d);
  Value g = power(&e, &power_val);
  g.grad = 1.0;

  g.backward(&g);
  print_val(&g);
  print_val(&e);
  print_val(&x);
  print_val(&c);
  print_val(&a);
  print_val(&x);
}