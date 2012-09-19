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
bool RootGraph::WriteToFile(const std::string& filename) {
  //Ensure that we can write to filename.
  std::ofstream outFile;
  outFile.open(filename.c_str(), std::ios::out);

  if (outFile.fail()) return false;

  Print(outFile);

  outFile.close();

  return true;
}

void RootGraph::Print(std::ostream& out, unsigned tabDepth) {
  out << (IsDigraph() ? "digraph " : "graph ") << GetId() << " {\n";
  std::string linePrefix = std::string(tabDepth*_tabIncrement, _tabCharacter);

  if (!_attributes.Empty()) {
    out << linePrefix << "graph [";
    _attributes.Print(out);
    out << "];\n";
  }

  PrintNECS(out, tabDepth);

  out << "}\n";
}

}  // namespace DotWriter
