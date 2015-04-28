//Layer of a neural network
#include "layer.hpp"

namespace neural
{
  //Constructs a new empty Layer
  Layer::Layer(unsigned bias_in)
  {
    bias = bias_in;
    //Create list of neurons
    neurons = std::vector<Neuron>();
  }

  //Constructs a new Layer of the specified size
  Layer::Layer(unsigned neurons_in, unsigned bias_in)
  {
    unsigned neuronIterator;

    bias = bias_in;
    //Create the list of neurons
    neurons = std::vector<Neuron>();
    //Add the new neurons to the layer
    for (neuronIterator = 0; neuronIterator < neurons_in; ++neuronIterator) {
      addNeuron(0);
    }
    //Add the bias neurons to the layer
    for (neuronIterator = 0; neuronIterator < bias_in; ++neuronIterator) {
      addNeuron(1);
    }
  }

  //Create and add a new neuron to the layer
  void Layer::addNeuron(unsigned bias_in)
  {
    neurons.push_back(Neuron(bias_in));
  }

  //Create and add a new neuron with the specified inputs to the layer
  void Layer::addNeuron(std::vector<Neuron>* inputs_in, unsigned bias_in)
  {
    neurons.push_back(Neuron(inputs_in, bias_in));
  }

  //Sets the values of the first neurons to the specified values
  void Layer::setValues(const std::vector<double> &values_in)
  {
    unsigned inputIterator;

    //Hit each neuron in the layer
    for (inputIterator = 0; inputIterator < (unsigned) values_in.size(); ++inputIterator) {
      neurons[inputIterator].setOutput(values_in[inputIterator]);
    }
  }

  //Sets all the neurons to forward their values for computation at the next layer
  void Layer::feedForward(double (*activationFunction)(double))
  {
    unsigned neuronIterator;

    //Hit each neuron in the layer
    for (neuronIterator = 0; neuronIterator < (unsigned) neurons.size() - bias; ++neuronIterator) {
      neurons[neuronIterator].feedForward(activationFunction);
    }
  }

  //Calculates the error for the network using root mean square storing it in the error member
  double Layer::calculateError(const std::vector<double> &values_in)
  {
    unsigned neuronIterator;
    double delta;
    double error;

    //Hit each neuron in the output layer
    for (neuronIterator = 0; neuronIterator < neurons.size() - bias; ++neuronIterator) {
      //Caucluate the difference for the current Neuron
      delta = values_in[neuronIterator] - neurons[neuronIterator].getOutput();
      //Add the error of this Neuron to the error
      error += delta * delta;
    }
    //Calculate root mean square
    return sqrt(error / (neurons.size() - 1));
  }

  //Calculates the gradients for the layer's neurons
  void Layer::calculateOutputGradients(const std::vector<double> &values_in, double (*activationFunctionDerivative)(double))
  {
    unsigned neuronIterator;

    //Hit each neuron in the output layer
    for (neuronIterator = 0; neuronIterator < neurons.size() - bias; ++neuronIterator) {
      //Calculate the gradients for that neuron
      neurons[neuronIterator].calculateOutputGradients(values_in[neuronIterator], activationFunctionDerivative);
    }
  }

  //Calculates the gradients for the layer
  void Layer::calculateHiddenGradients(double (*activationFunctionDerivative)(double))
  {
    unsigned neuronIterator;

    //Hit each neuron in the layer
    for (neuronIterator = 0; neuronIterator < neurons.size() - bias; ++neuronIterator) {
      //Calculate the gradients for that neuron
      neurons[neuronIterator].calculateHiddenGradients(activationFunctionDerivative);
    }
  }

  //Updates the weights of each neuron in the layer
  void Layer::updateInputWeights(double (*deltaInputWeight)(double, double, double, double))
  {
    unsigned neuronIterator;

    //Hit each neuron in the layer
    for (neuronIterator = 0; neuronIterator < neurons.size() - bias; ++neuronIterator) {
      //Update weights for the current neuron
      neurons[neuronIterator].updateInputWeights(deltaInputWeight);
    }
  }

  //Returns the result values of the layer
  void Layer::getResults(std::vector<double>* location_in)
  {
    unsigned resultIterator;

    //Empty the result vector
    location_in->clear();
    for (resultIterator = 0; resultIterator < neurons.size(); ++resultIterator) {
      //Ignore bias neurons
      if (neurons[resultIterator].isBias()) {
        continue;
      }
      //Append the neurons value to the results
      location_in->push_back(neurons[resultIterator].getOutput());
    }
  }

  //Modifies the value sof the neuron to match the input values
  void Layer::setNeuron(neuron_data& neuron_in)
  {
    //Set value of specified neuron
    neurons[neuron_in.neuron.neuron].setValues(neuron_in);
  }

  //Changes the values of all the bias neurons in the layer
  void Layer::setBias(double value_in)
  {
    unsigned neuronIterator;

    //Hit each bias neuron
    for (neuronIterator = neurons.size() - 1; neuronIterator >= neurons.size() - bias; --neuronIterator) {
      //Set the neurons value to the specified
      neurons[neuronIterator].setOutput(value_in);
    }
  }

  //Returns the neuron at the specified location
  Neuron* Layer::getNeuron(unsigned neuron_in)
  {
    return &neurons[neuron_in - 1];
  }

  unsigned Layer::numNeurons() { return neurons.size(); }
  std::vector<Neuron>* Layer::getNeurons() { return &neurons; }
}