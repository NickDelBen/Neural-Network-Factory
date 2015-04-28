/***********************************************************
* Reader for parsing neural networks stored as a json object
*
* Created By: Nick DelBen
* Created On: April 27, 2015
*
* Last Modified: April 27, 2015
*   - Created Initially
***********************************************************/

#ifndef _H_NEURAL_READER
#define _H_NEURAL_READER

#include <stdexcept>   //std::runtime_error
#include <stdio.h>     //FILE    fopen()    fclose()
#include <limits>      //std::numeric_limits<double>::quiet_NaN()

#include "../lib/rapidjson/filereadstream.h"
#include "../lib/rapidjson/document.h"

#include "neuron_id.hpp"
#include "neuron_data.hpp"
#include "connection_data.hpp"

#define READ_BUFFER_SIZE 65536

#define NaN std::numeric_limits<double>::quiet_NaN()

class Reader
{
private:
  /* File document is created from */
  FILE* sourceFile;

  /* Document object for parsing json */
  rapidjson::Document jsonDocument;

  /* Buffer object for holding document source */
  char readBuffer[READ_BUFFER_SIZE];

  /* Pointer to network topology data */
  rapidjson::Value* topology;

  /* Pointer to network neuron data */
  rapidjson::Value* neurons;

  /* Pointer to network connection data */
  rapidjson::Value* connections;

  /* Number of layers in the document */
  unsigned numLayers;

  /* Number of neurons in document */
  unsigned numNeurons;

  /* Number of connections in document */
  unsigned numConnections;

  /* Processed Layers */
  unsigned processedLayers;

  /* Processed Neurons */
  unsigned processedNeurons;

  /* Processed Connections */
  unsigned processedConnections;

  /*****************
  * Sets the values for this object
  * @param file_in file to read from
  *****************/
  void setValues(FILE* file_in);

public:
  /*****************
  * Creates a new reader from the specified file
  * @param file_in file to read from
  *****************/
  Reader(FILE* file_in);

  /*****************
  * Checks if there are unprocessed layers in the document
  * @return 0 All layers have been parsed
  * @return 1 Unparsed layers remain
  *****************/
  unsigned hasLayer();

  /*****************
  * Checks if there are unprocessed neurons in the document
  * @return 0 All neurons have been parsed
  * @return 1 Unparsed Neurons remain
  *****************/
  unsigned hasNeuron();

  /*****************
  * Checks if there are unprocessed connections in the document
  * @return 0 All connections have been parsed
  * @return 1 Unparsed connections remain
  *****************/
  unsigned hasConnection();

  /*****************
  * Parses the next unparsed layer
  * @return amount of neurons in next layer
  *****************/
  unsigned getLayer();

  /*****************
  * Parses the next unparsed neuron and stores data in specified location
  * @param location_in Location to store data
  *****************/
  void getNeuron(neuron_data* location_in);

  /*****************
  * Parses the next unparsed connection and stores data in specified location
  * @param location_in Location to store data
  *****************/
  void getConnection(connection_data* location_in);
};

#endif