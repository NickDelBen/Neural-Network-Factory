/***********************************************
* Connection between Neurons in a Neural Network.
*
* Created By: Nick DelBen
* Created On: March 12, 2015
* 
* Last Modified:
*   March 14, 2015 - Added include guard
***********************************************/

#ifndef _H_NEURAL_CONNECTION
#define _H_NEURAL_CONNECTION

namespace neural
{
  class Neuron;

  class Connection
  {
  private:
    /* Weight of the connection */
    double weight;
    /* */
    double deltaWeight;
    /* Neuron at receiving end of connection */
    Neuron* endpoint;
    /* Neuron at origin of connection */
    Neuron* start;

  public:
    /****************
    * Creates a new connection with the specified weight
    * @param start_in  Neuron outputting to this connection
    * @param end_in    Neuron inputting from this connection 
    * @param weight_in Weight of this connection
    ****************/
    Connection(Neuron* start_in, Neuron* end_in, double weight_in);

    /****************
    * Creates a new connection
    * @param start_in  Neuron outputting to this connection
    * @param end_in    Neuron inputting from this connection 
    ****************/
    Connection(Neuron* start_in, Neuron* end_in);

    /****************
    * Creates a new connection
    ****************/
    Connection();

    void setWeight(double weight_in);
    double getWeight() const;
    void setDeltaWeight(double weight_in);
    double getDeltaWeight() const;
    void setStart(Neuron* start_in);
    Neuron* getStart() const;
    void setEndpoint(Neuron* endpoint_in);
    Neuron* getEndpoint() const;
  };
}


#endif