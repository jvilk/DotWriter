/**
 * Represents the root graph of the DOT file.
 *
 * Author: John Vilk (jvilk@cs.umass.edu)
 */

#ifndef DOTWRITER_ROOTGRAPH_H_
#define DOTWRITER_ROOTGRAPH_H_

#include "AttributeSet.h"
#include "Graph.h"

namespace DotWriter {

class RootGraph : Graph {
private:
  GraphAttributeSet _attributes;

public:
  RootGraph(bool isDigraph = false) : Graph(new IdManager(), isDigraph),
    _attributes(GraphAttributeSet()) {};
  RootGraph(bool isDigraph, const std::string& label) :
    Graph(new IdManager(), isDigraph, label), _attributes(GraphAttributeSet())
  {};

  virtual ~RootGraph() {
    delete _idManager;
  }

  GraphAttributeSet& GetAttributes() {
    return _attributes;
  }

  /**
   * Writes the graph to the specified filename in the DOT format.
   *
   * Returns true if successful, false otherwise.
   */
  bool WriteToFile(std::string& filename);

  virtual void Print(std::ostream& out);
};

}  // namespace DotWriter

#endif