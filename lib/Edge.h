#ifndef DOTWRITER_EDGE_H_
#define DOTWRITER_EDGE_H_

#include "AttributeSet.h"

namespace DotWriter {

/**
 * Represents an edge in a graph.
 */
class Edge {
private:
  Node * _src;
  Node * _dst;
  std::string _label;
  EdgeAttributeSet _attributes;

public:
  Edge(Node * src, Node * dst) : _src(src), _dst(dst), _label("") {
  }

  Edge(Node * src, Node * dst, const std::string& label) : _src(src), _dst(dst),
    _label(label) {
  }

  virtual ~Edge() {};

  Node * GetSource() {
    return _src;
  }

  Node * GetDest() {
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
