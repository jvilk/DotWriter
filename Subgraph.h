/*
 * Subgraph class. Allows you to specify graphs within graphs for clustering
 * purposes.
 *
 * TODO(jvilk): May want to make Graph a subtype of this...
 *
 * Author: John Vilk (jvilk@cs.umass.edu)
 */

#ifndef DOTWRITER_SUBGRAPH_H_
#define DOTWRITER_SUBGRAPH_H_

#include "Node.h"
#include "Edge.h"
#include "AttributeSet.h"

#include <vector>

namespace DotWriter {

class Subgraph {
private:
  const std::string& _id;
  std::vector<Node*> _nodes;
  std::vector<Edge*> _edges;
  std::vector<Subgraph*> _subgraphs;
  SubgraphAttributeSet _attributes;
  NodeAttributeSet _defaultNodeAttributes;
  EdgeAttributeSet _defaultEdgeAttributes;

protected:
  // We protect the Subgraph constructor because it should only be called by
  // Graph.
  Subgraph(const std::string& id, std::string label = "") : _id(id),
    _attributes(SubgraphAttributeSet()),
    _defaultNodeAttributes(NodeAttributeSet()),
    _defaultEdgeAttributes(EdgeAttributeSet()) {
      // TODO(jvilk): Deal with label.
  }

public:
  /** Getters and setters **/
  const std::string& GetId() {
    return _id;
  }

  // TODO(jvilk): Deal with these.
  const std::string& GetLabel();

  void SetLabel(std::string label);


  Node* AddNode();

  void RemoveNode(Node* node);

  Edge* AddEdge();

  void RemoveEdge(Edge* edge);

  Subgraph* AddSubgraph();

  void RemoveSubgraph(Subgraph* subgraph);
};

}  // namespace DotWriter

#endif