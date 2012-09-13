#include "RootGraph.h"

#include <iostream>
#include <fstream>

#include "Subgraph.h"

namespace DotWriter {

/**
 * Outputs the graph as a dot file to the given filepath.
 * The path can be relative or absolute.
 *
 * Returns false if the operation fails, e.g. due to being unable to open the file.
 */
bool RootGraph::WriteToFile(std::string& filename) {
  //Ensure that we can write to filename.
  std::ofstream outFile;
  outFile.open(filename.c_str(), std::ios::out);

  if (outFile.fail()) return false;

  Print(outFile);

  outFile.close();

  return true;
}

void RootGraph::Print(std::ostream& out) {
  out << "digraph " << GetId() << " {\n";

  if (!_attributes.Empty()) {
    out << "graph [";
    _attributes.Print(out);
    out << "];\n";
  }

  PrintNECS(out);

  out << "}\n";
}

}  // namespace DotWriter