#Compiler to use for the make
cc=g++
#Location to store object files
DO=obj
#Directory for main binaries
DB=bin
#Directory where source files are
DS=src
#Directory where docs are stored
DD=doc

#Compiler flags to use for debugging
FD=-Wall -g
#Compiler flags to use for object files
FO=$(FD) -c
#Compiler Flags to use for binaries
FB=$(FD)

#Tarball output file
TAR_FILE=neural.tar.gz

################################################
# Build Commands
################################################

all: net

#Remove any previously built files
clean:
	#Remove any objects from the object directory
	rm -rf $(DO)

#Removes any files other thatn the source from the directory
purge: clean
	#Remove any binaries from the output directory
	rm -rf $(DB)
	#Remove the source tarball if it exists
	rm -rf $(TAR_FILE)
	#Remove the documentation files
	rm -rf $(DD)

#Create the directory structure required for output
prep:
	#Create the objects directory
	mkdir -p $(DO)
	#Create output directory
	mkdir -p $(DB)

documentation:
	#Generating documentaton
	doxygen Doxyfile

#Tar the project to make it easier to move around
tarball:
	tar -zcvf $(TAR_FILE) Doxyfile Makefile src

################################################
# Executable Binaries
################################################

#Build Neural Network executable
net: prep connection.o neuron.o layer.o network.o driver.o reader.o writer.o
	#Building the Neural Network binary
	$(cc) $(FB) -o $(DB)/net $(DO)/driver.o $(DO)/neuron.o $(DO)/connection.o $(DO)/layer.o $(DO)/network.o $(DO)/reader.o $(DO)/writer.o

################################################
# Object Files
################################################

driver.o: prep $(DS)/driver.cpp
	#Compiling driver object
	$(cc) $(FO) -o $(DO)/driver.o $(DS)/driver.cpp

connection.o: prep $(DS)/neural_net/connection.cpp
	#Compiling connection object
	$(cc) $(FO) -o $(DO)/connection.o $(DS)/neural_net/connection.cpp

neuron.o: prep $(DS)/neural_net/neuron.cpp
	#Compiling neuron object
	$(cc) $(FO) -o $(DO)/neuron.o $(DS)/neural_net/neuron.cpp

layer.o: prep $(DS)/neural_net/layer.cpp
	#Compiling layer object
	$(cc) $(FO) -o $(DO)/layer.o $(DS)/neural_net/layer.cpp

network.o: prep $(DS)/neural_net/network.cpp
	#Compiling neural net object
	$(cc) $(FO) -o $(DO)/network.o $(DS)/neural_net/network.cpp

reader.o: prep $(DS)/neural_net/reader.cpp
	#Compiling reader object
	$(cc) $(FO) -o $(DO)/reader.o $(DS)/neural_net/reader.cpp

writer.o: prep $(DS)/neural_net/writer.cpp
	#Compiling writer object
	$(cc) $(FO) -o $(DO)/writer.o $(DS)/neural_net/writer.cpp