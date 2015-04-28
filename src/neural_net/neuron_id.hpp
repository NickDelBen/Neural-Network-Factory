//Simple structure to identify a neuron

#ifndef _H_NEURAL_NEURON_ID
#define _H_NEURAL_NEURON_ID

typedef struct {
  unsigned layer;  //Layer the neuron is in
  unsigned neuron; //Index of neuron in layer
} neuron_id;

#endif