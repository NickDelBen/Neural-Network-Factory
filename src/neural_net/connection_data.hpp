//Simple structure to store data for a parsed connection

#ifndef _H_NEURAL_CONNECTION_DATA
#define _H_NEURAL_CONNECTION_DATA

#include "neuron_id.hpp"

typedef struct {
  neuron_id source;
  neuron_id destination;
  double weight;
  double deltaWeight;
} connection_data;

#endif