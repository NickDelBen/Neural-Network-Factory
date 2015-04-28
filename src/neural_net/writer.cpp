//Writer for saving a neural network as a json object

#include "writer.hpp"

//Creates a new writer
Writer::Writer() {}

//Creates a new reader from the specified file
Writer::Writer(FILE* file_in)
{
  //Set the values for the writer
  setValues(file_in);
}

//Sets the values for this object
void Writer::setValues(FILE* file_in)
{
  //Store specified file
  destinationFile = file_in;

  //Store allocator
  allocator = &jsonDocument.GetAllocator();

  jsonDocument.SetObject();

  //Create network data object
  rapidjson::Value network_new(rapidjson::kObjectType);
  //Create data arrays
  rapidjson::Value topology_new(rapidjson::kArrayType);
  rapidjson::Value neurons_new(rapidjson::kArrayType);
  rapidjson::Value connections_new(rapidjson::kArrayType);

  //Store json objects
  network = network_new;
  topology = topology_new;
  neurons = neurons_new;
  connections = connections_new;
}

//Adds a layer to the topology
void Writer::addLayer(unsigned neurons_in)
{
  //Add layer with specified neurons to layer array
  topology.PushBack(rapidjson::Value().SetInt(neurons_in), *allocator);
}

//Adds a neuron to the network
void Writer::addNeuron(neuron_data& neuron_in)
{
  //Create new object for neuron
  rapidjson::Value neuron_new(rapidjson::kObjectType);

  //Add the id of the neuron
  neuron_new.AddMember("layer", rapidjson::Value().SetInt(neuron_in.neuron.layer + 1), *allocator);
  neuron_new.AddMember("neuron", rapidjson::Value().SetInt(neuron_in.neuron.neuron + 1), *allocator);
  //If the specified neuron has a bias store it
  if (! std::isnan(neuron_in.bias)) {
    neuron_new.AddMember("bias", rapidjson::Value().SetInt(neuron_in.bias), *allocator);
  }
  //If the specified neuron has an output store it
  if (! std::isnan(neuron_in.output)) {
    neuron_new.AddMember("output", rapidjson::Value().SetDouble(neuron_in.bias), *allocator);
  }
  //If specified neuron has gradient store it
  if (! std::isnan(neuron_in.gradient)) {
    neuron_new.AddMember("gradient", rapidjson::Value().SetDouble(neuron_in.gradient), *allocator);
  }
  //Add new neuron to collection
  neurons.PushBack(neuron_new, *allocator);
}

//Adds a connection to the network
void Writer::addConnection(connection_data& connection_in)
{
  //Create new object for the connection
  rapidjson::Value connection_new(rapidjson::kObjectType);

  //Add id of source neuron
  connection_new.AddMember("sourceLayer", rapidjson::Value().SetInt(connection_in.source.layer + 1), *allocator);
  connection_new.AddMember("sourceNeuron", rapidjson::Value().SetInt(connection_in.source.neuron + 1), *allocator); 
  //Add id of destination neuron
  connection_new.AddMember("destLayer", rapidjson::Value().SetInt(connection_in.destination.layer + 1), *allocator);
  connection_new.AddMember("destNeuron", rapidjson::Value().SetInt(connection_in.destination.neuron + 1), *allocator); 
  //If specified connection has weight store it
  if (! std::isnan(connection_in.weight)) {
    connection_new.AddMember("weight", rapidjson::Value().SetDouble(connection_in.weight), *allocator);
  }  
  //If specified neuron has delta weight store it
  if (! std::isnan(connection_in.deltaWeight)) {
    connection_new.AddMember("deltaWeight", rapidjson::Value().SetDouble(connection_in.deltaWeight), *allocator);
  }
  //Add new connection to collection
  connections.PushBack(connection_new, *allocator);
}

//Commits the layers to the output document
void Writer::commitTopology()
{
  //Add topology object to network object
  network.AddMember("topology", topology, *allocator);
}

//Commits the neurons to the output document
void Writer::commitNeurons()
{
  //Add neurons object to network object
  network.AddMember("neurons", neurons, *allocator);
}

//Commits the connections to the output document
void Writer::commitConnections()
{
  //Add connections object to network object
  network.AddMember("connections", connections, *allocator);
}

//Writes the commited values to the specified value
void Writer::write()
{
  char writeBuffer[WRITE_BUFFER_SIZE];

  //Add network object to document
  jsonDocument.AddMember("network", network, *allocator);

  //Create stream from destination file
  rapidjson::FileWriteStream stream_out(destinationFile, writeBuffer, sizeof(writeBuffer));
  //Create file writer from output stream 
  rapidjson::Writer<rapidjson::FileWriteStream> jsonWriter(stream_out);
  //Write document contents to file
  jsonDocument.Accept(jsonWriter);
}