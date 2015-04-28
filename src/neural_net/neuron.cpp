//Neuron in a neural network
#include "neuron.hpp"


#include <iostream>

namespace neural
{
  //Creates a new neuron without inputs
  Neuron::Neuron(unsigned bias_in)
  {
    bias = bias_in;
  }

  //Creates a new neuron with the specified number of outputs
  Neuron::Neuron(std::vector<Neuron>* inputs_in, unsigned bias_in)
  {
    addInputs(inputs_in);
    bias = bias_in;
  }

  //Modifies the value sof the neuron to match the input values
  void Neuron::setValues(neuron_data& neuron_in)
  {
    //If the specified neuron has a bias store it
    if (! std::isnan(neuron_in.bias)) {
      setBias(neuron_in.bias);
    }
    //If the specified neuron has an output store it
    if (! std::isnan(neuron_in.output)) {
      setOutput(neuron_in.output);
    }
    //If specified neuron has gradient store it
    if (! std::isnan(neuron_in.gradient)) {
      setGradient(neuron_in.gradient);
    }
  }

  //Adds a collection of incoming connections to the Neuron
  void Neuron::addInputs(std::vector<Neuron>* inputs_in)
  {
    unsigned inputIterator;

    //Create the connections to the neuron
    for (inputIterator = 0; inputIterator < inputs_in->size(); ++inputIterator) {
      addInput(&(*inputs_in)[inputIterator]);
    }
  }

  //Create a new input connection from the specified Neuron
  void Neuron::addInput(Neuron* input_in)
  {
    //Create a new connection for the current input
    Connection newConnection = Connection(input_in, this, makeWeight());
    //Add new connection as input to node
    addInput(&newConnection);
  }

  //Adds an incoming connection to the Neuron
  void Neuron::addInput(Connection* input_in) {    
    //Put new connection to end of input collection
    inputs.push_back(input_in);
    //Add new connection as output for the input Neuron
    input_in->getStart()->addOutput(input_in);
  }

  //Adds an outgoing connection to the Neuron
  void Neuron::addOutput(Connection* output_in) {
    outputs.push_back(*output_in);
  }

  //Generates a weight for a new neuron
  double Neuron::makeWeight()
  {
    return rand() / double(RAND_MAX);
  }

  void Neuron::feedForward(double (*activationFunction)(double))
  {
    double sum;
    unsigned neuronIterator;

    sum = 0.0;

    //Sum all input layers (including bias)
    for (neuronIterator = 0; neuronIterator < inputs.size(); ++neuronIterator) {
      sum += inputs[neuronIterator]->getStart()->getOutput() * inputs[neuronIterator]->getWeight();
    }

    outputValue = activationFunction(sum);
  }

  //Calculates the output gradient for the neuron
  void Neuron::calculateOutputGradients(double value_in, double (*activationFunctionDerivative)(double))
  {
    gradient = (value_in - outputValue) * activationFunctionDerivative(outputValue);
  }

  //Calculates gradients for hidden neuron
  void Neuron::calculateHiddenGradients(double (*activationFunctionDerivative)(double))
  {
    gradient = sumDOW() * activationFunctionDerivative(outputValue);
  }

  //Finds sum of derivitaves of weights for outputs
  double Neuron::sumDOW()
  {
    unsigned outputIterator;
    Connection* currentConnection;
    double sum;
    
    //Hit each of the Neuron's outputs
    for (outputIterator = 0; outputIterator < outputs.size(); ++outputIterator) {
      currentConnection = &outputs[outputIterator];
      //IF bias neurons are reached stop
      if (currentConnection->getEndpoint()->isBias()) {
        continue;
      }
      sum += currentConnection->getWeight() * currentConnection->getEndpoint()->getGradient();
    }
    return sum;
  }

  //Updates the weights
  void Neuron::updateInputWeights(double (*deltaInputWeight)(double, double, double, double))
  {
    unsigned inputIterator;
    Connection* currentConnection;
    double newDeltaWeight;

    //Hit each input connection
    for (inputIterator = 0; inputIterator < inputs.size(); ++inputIterator) {
      currentConnection = inputs[inputIterator];
      //Calculate new deltaweight
      newDeltaWeight = deltaInputWeight(gradient, currentConnection->getWeight(), currentConnection->getDeltaWeight(), currentConnection->getStart()->getOutput());
      //Store the new delta weight and update the weight
      currentConnection->setDeltaWeight(newDeltaWeight);
      currentConnection->setWeight(currentConnection->getWeight() + newDeltaWeight);
    }
  }

  //Stores the neuron's data in the specified location
  void Neuron::getData(neuron_data* location_in)
  {
    location_in->bias = bias;
    location_in->gradient = gradient;
    location_in->output = outputValue;
  }

  //Getters and setters
  unsigned Neuron::isBias() const { return bias; }
  double Neuron::getGradient() const { return gradient; }
  void Neuron::setGradient(double gradient_in) { gradient = gradient_in; }
  void Neuron::setOutput(double value_in) { outputValue = value_in; }
  double Neuron::getOutput() const { return outputValue; }
  void Neuron::setBias(double bias_in) { bias = bias_in; }
}