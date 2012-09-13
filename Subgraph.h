/*
 * Subgraph class. Allows you to specify graphs within graphs for clustering
 * purposes.
 *
 * Author: John Vilk (jvilk@cs.umass.edu)
 */

#ifndef DOTWRITER_SUBGRAPH_H_
#define DOTWRITER_SUBGRAPH_H_

#include "AttributeSet.h"
#include "Graph.h"

namespace DotWriter {

class Subgraph : Graph {
private:
  SubgraphAttributeSet _attributes;

public:
  Subgraph(const std::string& id, IdManager* idManager, bool isDigraph = false,
    std::string label = "") : Graph(idManager, isDigraph, label, id),
    _attributes(SubgraphAttributeSet()) {
  }

  virtual ~Subgraph() {};

  /** Getters and setters **/
  SubgraphAttributeSet& GetAttributes() {
    return _attributes;
  }

  virtual void Print(std::ostream& out);
};

}  // namespace DotWriter

#endif