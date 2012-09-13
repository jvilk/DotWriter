#include "Edge.h"
#include "Node.h"

#include <ostream>

namespace DotWriter {
  
void Edge::Print(bool isDirected, std::ostream& out) {
  out << _src->GetId() << (isDirected ? "->" : "--") << _dst->GetId();

  if (!_attributes.Empty()) {
    out << " [";
    _attributes.Print(out);
    out << "]";
  }

  out << ";\n";
}

}  // namespace DotWriter
