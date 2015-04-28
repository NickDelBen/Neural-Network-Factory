//Reader for parsing neural networks stored as a json object

#include "reader.hpp"

//Creates a new reader from the specified fil
Reader::Reader(FILE* file_in)
{
  //Set the values for the object
  setValues(file_in);
}

void Reader::setValues(FILE* file_in)
{
  //Store document source file
  sourceFile = file_in;

  //Create stream from source file
  rapidjson::FileReadStream stream_in(file_in, readBuffer, sizeof(readBuffer));
  //Parse stream into document
  jsonDocument.ParseStream(stream_in);

  //Check to make sure document is valid
  if (! jsonDocument.HasMember("network")) {
    throw std::runtime_error("No network object found");
  }
  if (! jsonDocument["network"].HasMember("topology")) {
    throw std::runtime_error("No topology data found");
  }

  //Initilize processed numbers
  processedLayers = 0;
  processedNeurons = 0;
  processedConnections = 0;

  //Store pointers to the data elements in the document
  topology = &jsonDocument["network"]["topology"];
  neurons = NULL;
  connections = NULL;
  if (jsonDocument["network"].HasMember("neurons")) {
    neurons = &jsonDocument["network"]["neurons"];
  }
  if (jsonDocument["network"].HasMember("connections")) {
    connections = &jsonDocument["network"]["connections"];
  }

  //Store the sizes of the data elements
  numLayers = topology->Size();
  numNeurons = neurons == NULL ? 0 : neurons->Size();
  numConnections = connections == NULL ? 0 : connections->Size();
}

//Checks if there are unprocessed layers in the document
unsigned Reader::hasLayer()
{
  return processedLayers != numLayers;
}

//Checks if there are unprocessed neurons in the document
unsigned Reader::hasNeuron()
{
  return neurons != NULL && processedNeurons != numNeurons;
}

//Checks if there are unprocessed connections in the document
unsigned Reader::hasConnection()
{
  return connections != NULL && processedConnections != numConnections;
}

//Parses the next unparsed layer
unsigned Reader::getLayer()
{
  //Check if there are layers to be read
  if (! hasLayer()) {
    throw std::runtime_error("No unparsed layer found");
  }
  //Advance to next layer and return the current layer
  return (*topology)[processedLayers++].GetInt();
}

//Parses the next unparsed neuron and stores data in specified location
void Reader::getNeuron(neuron_data* location_in)
{
  //Check if there are neurons to be processed
  if (! hasNeuron()) {
    throw std::runtime_error("No unparsed neuron found");
  }
  //Store the specified neuron
  rapidjson::Value& currentNeuron = (*neurons)[processedNeurons++];

  //Ensure neuron is valid
  if (! currentNeuron.HasMember("layer")) {
    throw std::runtime_error("Neuron has no layer");
  }
  if (! currentNeuron.HasMember("neuron")) {
    throw std::runtime_error("Neuron has no index");
  }

  //Extract the information from the stored neuron data
  location_in->neuron.layer = currentNeuron["layer"].GetInt() - 1;
  location_in->neuron.neuron = currentNeuron["neuron"].GetInt() - 1;
  location_in->bias = currentNeuron.HasMember("bias") ? currentNeuron["bias"].GetInt() : NaN;
  location_in->output = currentNeuron.HasMember("output") ? currentNeuron["output"].GetDouble() : NaN;
  location_in->gradient = currentNeuron.HasMember("gradient") ? currentNeuron["gradient"].GetDouble() : NaN;
}

//Parses the next unparsed connection and stores data in specified location
void Reader::getConnection(connection_data* location_in)
{
  //Check if there are connections to be processed
  if (! hasConnection()) {
    throw std::runtime_error("No unparsed connection found");
  }
  //Store the specified connection
  rapidjson::Value& currentConnection = (*connections)[processedConnections++];

  //Ensure neuron is valid
  if (! currentConnection.HasMember("sourceLayer")) {
    throw std::runtime_error("Source neuron has no layer");
  }
  if (! currentConnection.HasMember("sourceNeuron")) {
    throw std::runtime_error("Source neuron has no index");
  }
  if (! currentConnection.HasMember("destLayer")) {
    throw std::runtime_error("Destination neuron has no layer");
  }
  if (! currentConnection.HasMember("destNeuron")) {
    throw std::runtime_error("Destination neuron has no index");
  }

  //Extract the information from the stored neuron data
  location_in->source.layer = currentConnection["sourceLayer"].GetInt() - 1;
  location_in->source.neuron = currentConnection["sourceNeuron"].GetInt() - 1;
  location_in->destination.layer = currentConnection["destLayer"].GetInt() - 1;
  location_in->destination.neuron = currentConnection["destNeuron"].GetInt() - 1;
  location_in->weight = currentConnection.HasMember("weight") ? currentConnection["weight"].GetDouble() : NaN;
  location_in->deltaWeight = currentConnection.HasMember("deltaWeight") ? currentConnection["deltaWeight"].GetDouble() : NaN;
}