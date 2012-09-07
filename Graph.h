/*
 * Graph class. Allows you to create a directed graph of nodes, and output
 * it to a DOT file.
 *
 * Author: John Vilk (jvilk@cs.umass.edu)
 */

#ifndef DOTWRITER_GRAPH_H_
#define DOTWRITER_GRAPH_H_

#include "Enums.h"
#include "Edge.h"
#include "IdManager.h"

#include <set>
#include <string>
#include <vector>
#include <map>

/**
 * TODO: Have an enum of attributes. Default to 0 for each? Make sure the
 * default is sane (or EMPTY)
 * node->setAttribute(GNodeAttribute.BLAH, Value)
 *
 * Graph Attributes:
 *  * fontcolor
 * Node Attributes:
 *  * Style
 *  * fontcolor
 *  * Shape
 * Edge Attributes:
 *  * Style
 *  * fontcolor
 *  * arrowhead
 *  * arrowsize
 * Make a UGraph for undirected graphs.
 * Clusters?
 */

namespace DotWriter {

/**
 * Represents a graph. It's a bag of nodes and edges, basically.
 */
class Graph {
private:
  bool _isDigraph;
  IdManager _idManager;
  const std::string& _id;
  std::string _label;
  Color::e _fontColor;
  std::set<Node *> _nodes;
  std::set<Edge *> _edges;
  std::set<Graph *> _subgraphs;

public:
  Graph(bool isDigraph = false) :
    _isDigraph(isDigraph), _idManager(IdManager()) {
    _id = _idManager.ValidateCustomId("Graph");
  }

  /** Simple getters and setters **/

  bool isDigraph() {
    return _isDigraph;
  }

  void setFontColor(Color::e fontColor) {
    _fontColor = fontColor;
  }

  Color::e getFontColor() {
    return _fontColor;
  }

  const std::string& getId() {
    return _id;
  }

  const std::string& getLabel() {
    return _label;
  }

  void setLabel(std::string label) {
    _label = label;
  }

  /**
   * Constructs a Node object, adds it to the graph, and returns it.
   */
  Node * addNode();
  /**
   * Constructs a Node object with the given ID, adds it to the graph, and
   * returns it.
   *
   * If the given ID is not unique, it will be uniquified by appending a number
   * to the end of the ID. Try not to rely on this feature, though.
   */
  Node * addNode(std::string id);

  /**
   * Removes the node from the graph.
   *
   * Note that this function deletes the node object, and removes all edges
   * to/from it. Also, note that it is currently more expensive than you may
   * expect -- O(|E|).
   */
  void removeNode(Node *);

  /**
   * Add an edge to the graph. Returns a reference to the edge that can be
   * manipulated to change edge properties.
   */
  Edge * addEdge(Node * src, Node * dst, std::string label = NULL);

  /**
   * Removes the edge from the graph. Note that this also deletes the GEdge
   * object.
   */
  void removeEdge(Edge *);

  /**
   * Removes any edges from src to dst from the graph. Note that this version
   * of the function is currently O(|E|).
   *
   * If this is not a digraph, then removeEdge(src, dst) has the same semantics
   * as removeEdge(dst, src).
   */
  void removeEdge(Node * src, Node * dst);

  /**
   * Returns a string representation of the graph in DOT file format.
   */
  std::string toString();

  /**
   * Returns a string representation of the graph as a subgraph section
   * of a DOT file.
   */
  std::string toStringSubgraph();

  /**
   * Writes the graph to the specified filename in the DOT format.
   *
   * Returns true if successful, false otherwise.
   */
  bool writeToFile(std::string& filename);

};


}  // namespace DotWriter

#endif /* GRAPH_H_ */
