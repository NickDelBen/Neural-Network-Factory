//Neural Network
#include "network.hpp"

namespace neural
{
  //Creates a new Network
  Network::Network() {}

  //Constructs a new instance of a Neural Network from the specified topology
  Network::Network(const std::vector<unsigned> &topology_in, double (*activationFunction_in)(double), double (*activationFunctionDerivative_in)(double), double (*deltaInputWeight_in)(double, double, double, double))
  {
    unsigned numLayers;
    unsigned layerIterator;
    unsigned neuronIterator;

    //Extract number of layers in network
    numLayers = topology_in.size();

    //Create the input layer with 1 bias Neuron
    layers.push_back(Layer(topology_in[0], 1));

    //Create the layers of the netwok
    for (layerIterator = 1; layerIterator < numLayers; ++layerIterator) {
      //Create the new layer
      layers.push_back(Layer(1));
      //Fill layer with neurons adding
      for (neuronIterator = 0; neuronIterator < topology_in[layerIterator]; ++neuronIterator) {
        layers.back().addNeuron(layers[layerIterator - 1].getNeurons(), 0);
      }
    }

    //Store the networks activation function and it's derivative
    activationFunction = activationFunction_in;
    activationFunctionDerivative = activationFunctionDerivative_in;
    //Store the function for reweiching connections
    deltaInputWeight = deltaInputWeight_in;
  }

  //Sets all the neurons to forward their values for computation at the next layer
  void Network::feedForward(const std::vector<double> &values_in)
  {
    unsigned layerIterator;

    //Assign the specified values into the input neurons
    inputLayer()->setValues(values_in);

    //Forward propigate
    for (layerIterator = 1; layerIterator < layers.size(); ++layerIterator) {
      layers[layerIterator].feedForward(activationFunction);
    }
  }

  //Sets the neurons values using back-propigation
  void Network::backPropagation(const std::vector<double> &values_in)
  {
    unsigned layerIterator;

    //Calculate overall error
    error = outputLayer()->calculateError(values_in);

    //Calculate output layer gradients
    outputLayer()->calculateOutputGradients(values_in, activationFunctionDerivative);

    //Calculate hidden layer gradients
    for (layerIterator = layers.size() - 2; layerIterator > 0; --layerIterator) {
      //Calculate the hidden gradients using the next layer
      layers[layerIterator].calculateHiddenGradients(activationFunctionDerivative);
    }

    //Update connection weights for neurons
    for (layerIterator = layers.size() - 1; layerIterator > 0; --layerIterator) {
      layers[layerIterator].updateInputWeights(deltaInputWeight);
    }
  }

  //Finds results of the layer
  void Network::getResults(std::vector<double> &resultValues_in)
  {
    outputLayer()->getResults(&resultValues_in);
  }

  //Creates a connection between the specified neurons
  void Network::createConnection(unsigned sourceLayer_in, unsigned sourceNeuron_in, unsigned destLayer_in, unsigned destNeuron_in)
  {
    Neuron* source;
    Neuron* destination;

    source = layers[sourceLayer_in].getNeuron(sourceNeuron_in);
    destination = layers[destLayer_in].getNeuron(destNeuron_in);

    //Create a connection between the source and destenation neurons
    destination->addInput(source);
  }

  //Creates a connection between the specified neurons
  void Network::createConnection(connection_data& connection_in)
  {
    Neuron* source;
    Neuron* destination;

    //Get pointers to connected nodes
    source = layers[connection_in.source.layer].getNeuron(connection_in.source.neuron);
    destination = layers[connection_in.destination.layer].getNeuron(connection_in.destination.neuron);

    //Create new connection
    Connection newConnection(source, destination);

    //If specified connection has weight store it
    if (! std::isnan(connection_in.weight)) {
      newConnection.setWeight(connection_in.weight);
    }
    //If specified neuron has delta weight store it
    if (! std::isnan(connection_in.deltaWeight)) {
      newConnection.setDeltaWeight(connection_in.deltaWeight);
    }

    //Add new connection to network
    destination->addInput(&newConnection);
  }

  //Modifies the values of a neuron to match the input values
  void Network::setNeuron(neuron_data& neuron_in)
  {
    //Set value of specified neuron
    layers[neuron_in.neuron.layer].setNeuron(neuron_in);
  }

  //Returns a pointer to the a requested layer
  Layer* Network::getLayer(unsigned layer_in)
  {
    return &layers[layer_in - 1];
  }

  unsigned Network::numLayers() { return layers.size(); }
  Layer* Network::outputLayer() { return &layers.front(); }
  Layer* Network::inputLayer() { return &layers.back(); }
}