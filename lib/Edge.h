#ifndef DOTWRITER_EDGE_H_
#define DOTWRITER_EDGE_H_

#include "AttributeSet.h"
#include "Idable.h"

namespace DotWriter {

class Graph;

/**
 * Represents an edge in a graph.
 */
class Edge {
private:
  Idable * _src;
  Idable * _dst;
  std::string _label;
  EdgeAttributeSet _attributes;

public:
  Edge(Idable * src, Idable * dst, std::string label = "");

  virtual ~Edge() {};

  Idable * GetSource() {
    return _src;
  }

  Idable * GetDest() {
    return _dst;
  }

  const std::string& GetLabel() {
    return _label;
  }

  void SetLabel(const std::string& label) {
    _label = label;
  }

  /**
   * Returns the set of attributes for this edge.
   * Manipulate this object to change the style of this edge.
   */
  EdgeAttributeSet& GetAttributes() {
    return _attributes;
  }

  void Print(bool isDirected, std::ostream& out);
};

}  // namespace DotWriter

#endif
