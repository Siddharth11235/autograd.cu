// include/nn.h
#include "engine.h"
#ifndef NN_H
#define NN_H

typedef struct Neuron {
    int nin;
    Value* weights;
    Value bias;
    
} Neuron;


float generate_random(float left, float right);
Neuron create_neuron(int nin);
Value call_neuron(Neuron* neuron, Value* x);
void free_neuron(Neuron* neuron);
void print_neuron(Neuron* neuron);

typedef struct  Layer {
    int nin;
    int nout;
    Neuron* neurons;
} Layer;

Layer create_layer(int nin, int nout);
Value* call_layer(Layer* layer, Value* x);
void print_layer(Layer* layer);

#endif  