/***********************************************
* Layer of a neural network.
*
* Created By: Nick DelBen
* Created On: March 12, 2015
* 
* Last Modified:
*   March 14, 2015 - Made activation function constructor parameter
***********************************************/

#ifndef _H_NEURAL_LAYER
#define _H_NEURAL_LAYER

#include <vector>    //std::vector

#include "neuron.hpp"
#include "neuron_data.hpp"
#include "neuron_id.hpp"

namespace neural
{
  class Layer
  {
  private:
    /* Neurons in this layer */
    std::vector<Neuron> neurons;
    /* Number of bias neurons in the layer */
    unsigned bias;
  
  public:
    /***********************
    * Constructs a new empty Layer
    * @param bias_in    Number of bias Neurons to be added
    ***********************/
    Layer(unsigned bias_in);

    /***********************
    * Constructs a new Layer of the specified size
    * @param neurons_in Number of Neurons in the specified layer
    * @param bias_in    Number of bias Neurons to be added
    ***********************/
    Layer(unsigned neurons_in, unsigned bias_in);

    /***********************
    * Create and add a new neuron to the layer
    * @param bias_in flags if this neuron is a bias neuron
    ***********************/
    void addNeuron(unsigned bias_in);

    /***********************
    * Create and add a new neuron with the specified inpuits to the layer
    * @param inputs_in neurons that input to this neuron
    * @param bias_in flags if this neuron is a bias neuron
    ***********************/
    void addNeuron(std::vector<Neuron>* inputs_in, unsigned bias_in);

    /***********************
    * Modifies the value sof the neuron to match the input values
    * @param neuron_in Vallues to match
    ***********************/
    void setNeuron(neuron_data& neuron_in);

    /**********************
    * Sets the values of the first neurons to the specified values
    * @param values_in Values to set neurons to
    **********************/
    void setValues(const std::vector<double> &values_in);

    /***********************
    * Sets all the neurons to forward their values for computation at the next layer
    * @param activationFunction function to call to determine neuron output
    ***********************/
    void feedForward(double (*activationFunction)(double));

    /***********************
    * Calculates the error for the network using root mean square storing it in the error member
    * @param values_in Values expected for each Neuron
    * @return Error for the layer
    ***********************/
    double calculateError(const std::vector<double> &values_in);

    /***********************
    * Calculates the output gradients for the layer
    * @param values_in Values expected for each Neuron
    * @param activationFunctionDerivative derivative of activation function
    ***********************/
    void calculateOutputGradients(const std::vector<double> &values_in, double (*activationFunctionDerivative)(double));

    /***********************
    * Calculates the gradients for the layer
    * @param activationFunctionDerivative derivative of activation function
    ***********************/
    void calculateHiddenGradients(double (*activationFunctionDerivative)(double));

    /***********************
    * Updates the weights of each neuron in the layer
    ***********************/
    void updateInputWeights(double (*deltaInputWeight)(double, double, double, double));

    /***********************
    * Returns the result values of the layer
    * @param location_in lcoation to store the values
    ***********************/
    void getResults(std::vector<double>* location_in);

    /**********************
    * Changes the values of all the bias neurons in the layer
    * @param value_in New value for bias neurons
    **********************/
    void setBias(double value_in);

    /**********************
    * Returns the neuron at the specified location
    * @param neuron_in Neuron to return
    **********************/
    Neuron* getNeuron(unsigned neuron_in);

    unsigned numNeurons();
    std::vector<Neuron>* getNeurons();
  };
}

#endif