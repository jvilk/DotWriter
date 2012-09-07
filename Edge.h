#ifndef DOTWRITER_EDGE_H_
#define DOTWRITER_EDGE_H_

#include "Node.h"

namespace DotWriter {

/**
 * Represents an edge in a graph.
 */
class Edge {
private:
  Node * _src;
  Node * _dst;

protected:
  Edge(Node * src, Node * dst) {
    _src = src;
    _dst = dst;
  }

  virtual ~Edge();

public:
  Node * getSource() {
    return _src;
  }

  Node * getDest() {
    return _dst;
  }

  std::string toString() {
    return "";
  }
};

}  // namespace DotWriter

#endif