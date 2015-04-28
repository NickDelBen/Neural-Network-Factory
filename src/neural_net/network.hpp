/***********************************************
* Neural Network.
*
* Created By: Nick DelBen
* Created On: March 12, 2015
* 
* Last Modified:
*   March 14, 2015 - Made activation function constructor parameter
***********************************************/

#ifndef _H_NEURAL_NETWORK
#define _H_NEURAL_NETWORK

#include <vector>   //std::vector
#include <cmath>    //std::isnan()

#include "layer.hpp"
#include "neuron.hpp"
#include "connection.hpp"
#include "connection_data.hpp"

#define NEURAL_BIAS_NEURONS 1
#define NEURAL_BIAS_VALUE   1.0

namespace neural
{
  class Network
  {
  private:
    /* Layers of Neurons in the network */
    std::vector<Layer> layers;
    /* Function used when neurons fire */
    double (*activationFunction)(double);
    /* Derivative of function used when neurons fire */
    double (*activationFunctionDerivative)(double);
    /* Function to reweigh neuron connections */
    double (*deltaInputWeight)(double, double, double, double);
    /* Error of the network */
    double error;

  public:
    /***********************
    * Creates a new Network
    ***********************/
    Network();

    /***********************
    * Constructs a new instance of a Neural Network from the specified topology
    * @param topology vector with each element pertaining to the amount of neuraons at level index
    * @param activationFunction Function to call on neuron data should return [-1...1]
    ***********************/
    Network(const std::vector<unsigned> &topology_in, double (*activationFunction_in)(double), double (*activationFunctionDerivative_in)(double), double (*deltaInputWeight_in)(double, double, double, double));

    /***********************
    * Sets all the neurons to forward their values for computation at the next layer
    * @param values_in values for the input neurons
    ***********************/
    void feedForward(const std::vector<double> &values_in);

    /***********************
    * Sets the neurons values using back-propigation
    * @param values_in values to test against
    ***********************/
    void backPropagation(const std::vector<double> &values_in);    

    /***********************
    * Finds the current results of the network
    * @param resultValues_in location to store result values
    ***********************/
    void getResults(std::vector<double> &resultValues_in);

    /***********************
    * Modifies the values of a neuron to match the input values
    * @param neuron_in Vallues to match
    ***********************/
    void setNeuron(neuron_data& neuron_in);

    /***********************
    * Creates a connection between the specified neurons
    * @param sourceLayer_in  index of source layer
    * @param sourceNeuron_in index of neuron in source layer
    * @param destLayer_in    index of destination layer
    * @param destNueron_in   index of neuron in destination layer
    **********************/
    void createConnection(unsigned sourceLayer_in, unsigned sourceNeuron_in, unsigned destLayer_in, unsigned destNeuron_in);

    /**********************
    * Creates a connection between the specified neurons
    * @param connection_in Connection to be created
    **********************/
    void createConnection(connection_data& connection_in);

    /**********************
    * Returns a pointer to the a requested layer
    * @param layer_in layer top be retrieved
    **********************/
    Layer* getLayer(unsigned layer_in);

    unsigned numLayers();
    Layer* outputLayer();
    Layer* inputLayer();
  };
}

#endif