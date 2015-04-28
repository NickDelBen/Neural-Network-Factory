//Simple structure to store data for a parsed neuron

#ifndef _H_NEURAL_NEURON_DATA
#define _H_NEURAL_NEURON_DATA

#include "neuron_id.hpp"

typedef struct {
  neuron_id neuron;
  unsigned bias;
  double output;
  double gradient;
} neuron_data;

#endif