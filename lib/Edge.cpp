#include "Edge.h"
#include "Node.h"
#include "Graph.h"

#include <ostream>

namespace DotWriter {

Edge::Edge(Node * src, Node * dst, std::string label) : _src(src),
  _dst(dst), _label(label) {
}

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
