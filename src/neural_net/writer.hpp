/***********************************************************
* Writer for saving a neural network as a json object
*
* Created By: Nick DelBen
* Created On: April 27, 2015
*
* Last Modified: April 27, 2015
*   - Created Initially
***********************************************************/

#ifndef _H_NEURAL_WRITER
#define _H_NEURAL_WRITER

#include <stdio.h>     //FILE    fopen()    fclose()
#include <cmath>       //std::isnan()

#include "../lib/rapidjson/filewritestream.h"
#include "../lib/rapidjson/document.h"
#include "../lib/rapidjson/writer.h"

#include "neuron_id.hpp"
#include "neuron_data.hpp"
#include "connection_data.hpp"

#define WRITE_BUFFER_SIZE 65536

class Writer
{
private:
  /* File document is to be written to */
  FILE* destinationFile;

  /* Document object for holding json */
  rapidjson::Document jsonDocument;

  /* Pointer to network data object */
  rapidjson::Value network;

  /* Pointer to network topology data */
  rapidjson::Value topology;

  /* Pointer to network neuron data */
  rapidjson::Value neurons;

  /* Pointer to network connection data */
  rapidjson::Value connections;

  /* Allocator for the json object */
  rapidjson::Document::AllocatorType* allocator;

  /*****************
  * Sets the values for this object
  * @param file_in file to read from
  *****************/
  void setValues(FILE* file_in);

public:
  /*****************
  * Creates a new writer
  *****************/
  Writer();

  /*****************
  * Creates a new reader from the specified file
  * @param file_in file to read from
  *****************/
  Writer(FILE* file_in);

  /*****************
  * Adds a layer to the topology
  * @param neurons_in amount of neurons (including bias) in new layer
  *****************/
  void addLayer(unsigned neurons_in);

  /*****************
  * Adds a neuron to the network
  * @param neuron_in neuron to be added
  *****************/
  void addNeuron(neuron_data& neuron_in);

  /*****************
  * Adds a connection to the network
  * @param connection_in neuron to be added
  *****************/
  void addConnection(connection_data& connection_in);

  /*****************
  * Commits the layers to the output document
  *****************/
  void commitTopology();

  /*****************
  * Commits the neurons to the output document
  *****************/
  void commitNeurons();

  /*****************
  * Commits the connections to the output document
  *****************/
  void commitConnections();

  /*****************
  * Writes the commited values to the specified value
  *****************/
  void write();
};

#endif