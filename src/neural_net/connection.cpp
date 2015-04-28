//Connection between Neurons in a Neural Network
#include "connection.hpp"

namespace neural
{  
  //Creates a new connection with the specified weight
  Connection::Connection(Neuron* start_in, Neuron* const end_in, double weight_in)
  {
    weight = weight_in;
    start = start_in;
    endpoint = end_in;
  }

  //Creates a new connection
  Connection::Connection(Neuron* start_in, Neuron* end_in)
  {
    start = start_in;
    endpoint = end_in;
  }

  //Creates a new connection
  Connection::Connection() {}

  //Getters and Setters
  void Connection::setWeight(double weight_in) { weight = weight_in; }
  double Connection::getWeight() const { return weight; }
  void Connection::setDeltaWeight(double weight_in) { deltaWeight = weight_in; }
  double Connection::getDeltaWeight() const { return deltaWeight; }
  void Connection::setStart(Neuron* start_in) { start = start_in; }
  Neuron* Connection::getStart() const { return start; }
  void Connection::setEndpoint(Neuron* endpoint_in) { endpoint = endpoint_in; }
  Neuron* Connection::getEndpoint() const { return endpoint; }
}
