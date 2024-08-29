#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> 

#include "../include/engine.h"
#include "../include/nn.h"



float generate_random(float left, float right){
    float randomNumber = sin(rand() * rand());
    return left + (right - left) * fabs(randomNumber);
}

Value* modify_input(float* inputs, int nin) {
    Value* input_vals = (Value*)malloc(nin * sizeof(Value));
    for (int i = 0; i < nin; i++) {
        input_vals[i] = create_value(inputs[i]);
    }
    return input_vals;
}


Neuron create_neuron(int nin) {
    Neuron neuron;
    neuron.nin = nin;
    neuron.weights = (Value*)malloc(nin * sizeof(Value)); 
    for (int i = 0; i < nin; i++) {
        neuron.weights[i] = create_value(generate_random(-1, 1));
    }
    neuron.bias = create_value(0);
    return neuron;
};

Value call_neuron(Neuron* neuron, Value* x) {
    Value* sum = malloc(sizeof(Value));
    *sum = create_value(0); 

    for (int i = 0; i < neuron->nin; i++) {
        Value prod_result = prod(&neuron->weights[i], &x[i]); 
        Value temp = add(sum, &prod_result);

        free(sum);
        sum = malloc(sizeof(Value)); 
        *sum = temp;
    }


    Value output_val = relu(sum); 
    free(sum); 
    return output_val;
}

void free_neuron(Neuron* neuron) {
    free(neuron);
};
void print_neuron(Neuron* neuron) {
    printf("Weights: ");
    for (int i = 0; i < neuron->nin; i++) {
        print_val(&neuron->weights[i]);
    }
    printf("\n");
};


Layer create_layer(int nin, int nout) {
    Layer layer;
    layer.nin = nin;
    layer.nout = nout;
    layer.neurons = (Neuron*)malloc(nout * sizeof(Neuron)); 
    for (int i = 0; i < layer.nout; i++) {
        layer.neurons[i] = create_neuron(nin);
    }
    return layer;
}

Value* call_layer(Layer* layer, Value* x) {
    Value* output = (Value*)malloc(layer->nout * sizeof(Value));
    for (int i = 0; i < layer->nout; i++) {
        output[i] =  call_neuron(&layer->neurons[i], x);
    } 
    return output;
};

void print_layer(Layer* layer) {
    printf("Number of inputs: %d\n", layer->nin);
    for (int i = 0; i < layer->nout; i++) {
        print_neuron(&layer->neurons[i]);
    } 
    printf("\nNumber of outputs for this layer: %d\n", layer->nout);
};



int main() {
    srand(time(NULL)); 
    Layer layer = create_layer(5, 1);

    float x[5] = {0.45, 0.12, 0.34, 0.62, 0.22}; 
    Value* x_val = modify_input(x, 5); 
    Value* output = call_layer(&layer, x_val);

    print_layer(&layer);

    for (int i = 0; i < 1; i++) {
        print_val(&output[i]);
    }
    output[0].grad = 1;
    print_val(&output[0]);
    output[0].backward(&output[0]);

    print_val(&output[0]);

    
    free(output); 
    free(layer.neurons);
}
