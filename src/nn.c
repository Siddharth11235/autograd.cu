#include <stdio.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../include/engine.h"
#include "../include/nn.h"

float generate_random(float left, float right) {
    float randomNumber = sin(rand() * rand());
    return left + (right - left) * fabs(randomNumber);
}

Value** modify_input(float* inputs, int nin) {
    Value** input_vals = (Value**)malloc(nin * sizeof(Value*));
    for (int i = 0; i < nin; i++) {
        input_vals[i] = create_value(inputs[i]); 
    }
    return input_vals;  
}


Neuron* create_neuron(int nin) {
    

    Neuron* neuron = (Neuron*)malloc(sizeof(Neuron));
    neuron->nin = nin;
    neuron->weights = (Value**)malloc(nin * sizeof(Value*));
    for (int i = 0; i < nin; i++) {
        neuron->weights[i] = create_value(generate_random(-1, 1));
    }
    neuron->bias = create_value(0);
    return neuron;
}

Value* call_neuron(Neuron* neuron, Value** x) {
    Value* sum = create_value(0); 

    // calculate the weighted sum
    for (int i = 0; i < neuron->nin; i++) {
        Value* prod_result = prod(neuron->weights[i], x[i]);
        sum = add(sum, prod_result); 
    }

    Value* output_val = relu(sum); 
    return output_val; 
}

void free_neuron(Neuron* neuron) {
    if (neuron->weights) {
        free(neuron->weights);
    }
}

void print_neuron(Neuron* neuron) {
    printf("Weights: ");
    for (int i = 0; i < neuron->nin; i++) {
        print_val(neuron->weights[i]);
    }
    printf("\n");
}

Layer* create_layer(int nin, int nout) {
    Layer* layer = (Layer*)malloc(sizeof(Layer));
    layer->nin = nin;
    layer->nout = nout;
    layer->neurons = (Neuron**)malloc(nout * sizeof(Neuron*));
    for (int i = 0; i < layer->nout; i++) {
        layer->neurons[i] = create_neuron(nin);
    }
    return layer;
}

Value** call_layer(Layer* layer, Value** x) {
    Value** output = (Value**)malloc(layer->nout * sizeof(Value*));
    for (int i = 0; i < layer->nout; i++) {
        output[i] = call_neuron(layer->neurons[i], x);
    }
    return output;
}

void print_layer(Layer* layer) {
    printf("Number of inputs: %d\n", layer->nin);
    for (int i = 0; i < layer->nout; i++) {
        print_neuron(layer->neurons[i]);
    }
    printf("\nNumber of outputs for this layer: %d\n", layer->nout);
}

// Frees the neurons inside the layer.
void free_layer(Layer* layer) {
    for (int i = 0; i < layer->nout; i++) {
        free_neuron(layer->neurons[i]);
    }
    if (layer->neurons) {
        free(layer->neurons);
    }
}

int main() {
    srand(time(NULL));
    int num_out = 4;
    Layer* layer = create_layer(5, num_out);

    float x[5] = {0.45, 0.12, 0.34, 0.62, 0.22};
    Value** x_val = modify_input(x, 5);
    Value** output = call_layer(layer, x_val);

    print_layer(layer);

    for (int i = 0; i < num_out; i++) {
        print_val(output[i]);
        output[i]->grad = 1.0;
        print_val(output[i]);
        output[i]->backward(output[i]);
        print_neuron(layer->neurons[i]);
    }
    

    free(output);
    free_layer(layer);
    free(x_val); 
    return 0;
}
