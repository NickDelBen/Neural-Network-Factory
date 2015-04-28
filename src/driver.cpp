#include "neural_net/reader.hpp"
#include "neural_net/writer.hpp"
#include "neural_net/network.hpp"

#define TRAINING_RATE     0.15
#define TRAINING_MOMENTUM 0.5
static double deltaInputWeight(double neuronGradient_in, double weight_in, double deltaWeight_in, double inputNeuronValue_in)
{
  return TRAINING_RATE * inputNeuronValue_in * neuronGradient_in + TRAINING_MOMENTUM * deltaWeight_in;
}

static double activation(double value_in)
{
  return tanh(value_in);
}

static double activationDerivative(double value_in)
{
  return 1.0 - value_in * value_in;
}

void readNetwork(char* fileName, neural::Network* network_in)
{
  std::vector<unsigned> topology; //Defines the amount of layers, and amount of nodes at each layer
  FILE* file_in;                  //File to read network specifications from
  neuron_data pn;
  connection_data pc;

  //Open input file
  file_in = fopen("net1.json", "r");
  //Create reader
  Reader reader(file_in);

  //Read topology from file
  while (reader.hasLayer()) {
    topology.push_back(reader.getLayer());
  }

  //Create neural network from read topology
  *network_in = neural::Network(topology, activation, activationDerivative, deltaInputWeight);

  //Read file for neurons
  while (reader.hasNeuron()) {
    //Get the next unprocessed neuron
    reader.getNeuron(&pn);
    //Add new neuron to network
    network_in->setNeuron(pn);
  }  

  //Read file for connections
  while (reader.hasConnection()) {
    //Get the next unprocessed connection
    reader.getConnection(&pc);
    //Add new connection to network
    network_in->createConnection(pc);
  }

  

  fclose(file_in);
}

void writeNetwork(char* fileName, neural::Network* network_in)
{
  FILE* file_out;
  unsigned layerIterator;
  unsigned neuronIterator;
  unsigned layerSize;
  neuron_data neuron;

  //Open output file
  file_out = fopen("save1.json", "w");
  //Create writer
  Writer writer(file_out);
  
  //Hit each layer
  for (layerIterator = 1; layerIterator <= network_in->numLayers(); ++layerIterator) {
    neuron.neuron.layer = layerIterator;
    //Extract the size of the layer
    layerSize = network_in->getLayer(layerIterator)->numNeurons();
    //Add this layer to the topology
    writer.addLayer(layerSize);
    //Hit each neuron in the layer
    for (neuronIterator = 1; neuronIterator <= layerSize ; ++neuronIterator) {
      neuron.neuron.neuron = neuronIterator;
      //Extract the data from the neuron
      network_in->getLayer(layerIterator)->getNeuron(neuronIterator)->getData(&neuron);
      writer.addNeuron(neuron);
    }
  }

  //Commit the topology and neurons for writing
  writer.commitTopology();
  writer.commitNeurons();
  //Write data to file
  writer.write();

  fclose(file_out);
}

int main()
{
  neural::Network net;

  char inFile[] = "net1.json";
  char outFile[] = "test1.json";

  readNetwork(inFile, &net);
  writeNetwork(outFile, &net);
  
  /*
  std::vector<double>   inputValues;  //Values to train neural network input with forward-propagation
  std::vector<double>   targetValues; //Values to train neural network output with back-propagation
  std::vector<double>   resultValues; //Holds output from neural net  
  
  //Train network
  myNet.feedForward(inputValues);      //Input on input training data 
  myNet.backPropagation(targetValues); //Train on expected output with backpropagation

  myNet.getResults(resultValues);
  */

  return 0;
}