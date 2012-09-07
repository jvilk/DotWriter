/*
 * Implementation of the graph class and related classes.
 *
 * Author: John Vilk (jvilk@cs.umass.edu)
 */
#include "Graph.h"

#include <iostream>
#include <fstream>

namespace DotWriter {

/**
 * Outputs the graph as a dot file to the given filepath.
 * The path can be relative or absolute.
 *
 * Returns false if the operation fails, e.g. due to being unable to open the file.
 */
bool Graph::writeToFile(std::string& filename) {
	//Ensure that we can write to filename.
	std::ofstream outFile;
	outFile.open(filename.c_str(), std::ios::out);

	if (outFile.fail()) return false;

	outFile << "digraph " << getId() << " {\n";

	//Output nodes and their names.


	//Output edges and their labels.

	outFile.close();

	return true;
}

}  // namespace DotWriter