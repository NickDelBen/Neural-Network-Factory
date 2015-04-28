/***********************************************
* Neuron in a neural network.
*
* Created By: Nick DelBen
* Created On: March 12, 2015
* 
* Last Modified:
*   March 15, 2015 - Made activation function a parameter instead of hardcoding
***********************************************/

#ifndef _H_NEURAL_NEURON
#define _H_NEURAL_NEURON

#include <vector>    //std::vector
#include <cstdlib>   //rand()
#include <cmath>     //tanh()    std::isnan()

#include "connection.hpp"
#include "neuron_data.hpp"
#include "neuron_id.hpp"

namespace neural
{
  class Neuron
  {
  private:
    /* Value of the neuron */
    double outputValue;
    /* Connections that input to the Neuron */
    std::vector<Connection*> inputs;
    /* Connections that output to the neuron */
    std::vector<Connection> outputs;
    /* Value of the gradient for this Neuron */
    double gradient;
    /* Flags if this is a bias neuron */
    unsigned bias;

    /****************
    * Generates a weight for a new neuron
    * @return new weight
    ****************/
    double makeWeight();

    /****************
    * Finds sum of derivitaves of weights for outputs
    * @return sum
    ****************/
    double sumDOW();

  public:
    /****************
    * Creates a new neuron without inputs
    * @param bias_in flags if this neuron is a bias neuron
    ****************/
    Neuron(unsigned bias_in);

    /****************
    * Creates a new neuron with the specified number of outputs
    * @param inputs_in Intput paths for neuron
    * @param bias_in flags if this neuron is a bias neuron
    ****************/
    Neuron(std::vector<Neuron>* inputs_in, unsigned bias_in);

    /***********************
    * Modifies the value sof the neuron to match the input values
    * @param neuron_in Vallues to match
    ***********************/
    void setValues(neuron_data& neuron_in);

    /****************
    * Adds a collection of incoming connections to the Neuron
    * @param inputs_in Intput paths for neuron
    ****************/
    void addInputs(std::vector<Neuron>* inputs_in);
    
    /****************
    * Create a new input connection from the specified Neuron
    * @param input_in Input connection to be added
    ****************/
    void addInput(Neuron* input_in);
    
    /****************
    * Adds an incoming connection to the Neuron
    * @param input_in Input connection to be added
    ****************/
    void addInput(Connection* input_in);

    /****************
    * Adds an outgoing connection to the Neuron
    * @param outputon_in the connection to be added
    ****************/
    void addOutput(Connection* output_in);

    /****************
    * Trains the node based on the input nodes using forward-propagation
    * @param activationFunction function to call to determine neuron output
    ****************/
    void feedForward(double (*activationFunction)(double));

    /****************
    * Calculates gradient for output neuron
    * @param value_in expected value for the neuron
    * @param activationFunctionDerivative derivative of the Neuron's activation function
    ****************/
    void calculateOutputGradients(double value_in, double (*activationFunctionDerivative)(double));

    /****************
    * Calculates gradients for hidden neuron
    * @param activationFunctionDerivative derivative of the Neuron's activation function
    ****************/
    void calculateHiddenGradients(double (*activationFunctionDerivative)(double));

    /****************
    * Updates the weights of the connections
    * @param deltaInputWeight function that returns new weight for connection on each input
    *   PARAMETERS (in order)
    *     double - Gradient for this neuron
    *     double - Weight of the connection
    *     double - deltaWeight of the connection
    *     double - value of the neuron inputing to this connection
    ****************/
    void updateInputWeights(double (*deltaInputWeight)(double, double, double, double));

    /****************
    * Stores the neuron's data in the specified location
    * @param location_in location to copy the neuron data to
    ****************/
    void getData(neuron_data* location_in);

    unsigned isBias() const;
    double getGradient() const;
    void setGradient(double gradient_in);
    void setOutput(double value_in);
    double getOutput() const;
    void setBias(double bias_in);
  };
}

#endif