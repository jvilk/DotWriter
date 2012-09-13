#include "Cluster.h"

namespace DotWriter {

void Cluster::Print(std::ostream& out) {
  out << "subgraph " << GetId() << " {\n";

  if (!_attributes.Empty()) {
    out << "graph [";
    _attributes.Print(out);
    out << "];\n";
  }

  PrintNECS(out);

  out << "}\n";
}

}  // namespace DotWriter