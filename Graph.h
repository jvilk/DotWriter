/*
 * Graph class. Allows you to create a directed graph of nodes, and output
 * it to a DOT file.
 *
 * Author: John Vilk (jvilk@cs.umass.edu)
 */

#ifndef DOTWRITER_GRAPH_H_
#define DOTWRITER_GRAPH_H_

#include <set>
#include <string>
#include <vector>
#include <map>

#include "Enums.h"
#include "Edge.h"
#include "IdManager.h"
#include "Subgraph.h"
#include "AttributeSet.h"

namespace DotWriter {

/**
 * Represents a graph. It's a bag of nodes and edges, basically.
 */
class Graph {
private:
  bool _isDigraph;
  IdManager _idManager;
  const std::string& _id;  // Reference to unique ID stored in the _idManager.
                           // Not really all that important, as you don't
                           // reference this in the DOT file. Dotty seems to
                           // want one on the master graph regardless.
  // I use vector since output order matters.
  std::vector<Node *> _nodes;
  std::vector<Edge *> _edges;
  std::vector<Graph *> _subgraphs;
  GraphAttributeSet _attributes;
  NodeAttributeSet _defaultNodeAttributes;
  EdgeAttributeSet _defaultEdgeAttributes;

public:
  /**
   * Constructs a new Graph object.
   * - isDigraph: Set to 'true' if this is a directed graph.
   */
  Graph(bool isDigraph = false) :
    _isDigraph(isDigraph), _idManager(IdManager()),
    _id(_idManager.ValidateCustomId("Graph")),
    _attributes(GraphAttributeSet()),
    _defaultNodeAttributes(NodeAttributeSet()),
    _defaultEdgeAttributes(EdgeAttributeSet()) {
  }

  /**
   * Constructs a new Graph object.
   * - isDigraph: Set to 'true' if this is a directed graph.
   * - label: Text that is printed somewhere adjacent to the graph.
   */
  Graph(bool isDigraph, std::string label) :
    _isDigraph(isDigraph), _idManager(IdManager()),
    _id(_idManager.ValidateCustomId("Graph")),
    _attributes(GraphAttributeSet()),
    _defaultNodeAttributes(NodeAttributeSet()),
    _defaultEdgeAttributes(EdgeAttributeSet()) {
      // TODO(jvilk): Deal with label.
  }

  /** Simple getters and setters **/

  bool IsDigraph() {
    return _isDigraph;
  }

  GraphAttributeSet& GetAttributes() {
    return _attributes;
  }

  EdgeAttributeSet& GetDefaultEdgeAttributes() {
    return _defaultEdgeAttributes;
  }

  NodeAttributeSet& GetDefaultNodeAttributes() {
    return _defaultNodeAttributes;
  }

  const std::string& GetId() {
    return _id;
  }

  /**
   * Create a new subgraph on this graph.
   */
  Subgraph* AddSubgraph() {
  }

  /**
   * Remove the given subgraph from this graph.
   */
  void RemoveSubgraph(Subgraph* subgraph);

  /**
   * Constructs a Node object, adds it to the graph, and returns it.
   */
  Node* AddNode();

  /**
   * Constructs a Node object with the given ID, adds it to the graph, and
   * returns it.
   *
   * If the given ID is not unique, it will be uniquified by appending a number
   * to the end of the ID.
   */
  Node* AddNode(std::string id);

  /**
   * Removes the node from the graph.
   *
   * Note that this function deletes the node object, and removes all edges
   * to/from it. Also, note that it is currently more expensive than you may
   * expect -- O(|E|).
   */
  void RemoveNode(Node* node);

  /**
   * Add an edge to the graph. Returns a reference to the edge that can be
   * manipulated to change edge properties.
   */
  Edge* AddEdge(Node* src, Node* dst);
  Edge* AddEdge(Node* src, Node* dst, std::string label);

  /**
   * Removes the edge from the graph. Note that this also deletes the GEdge
   * object.
   */
  void RemoveEdge(Edge* edge);

  /**
   * Removes any edges from src to dst from the graph. Note that this version
   * of the function is currently O(|E|).
   *
   * If this is not a digraph, then removeEdge(src, dst) has the same semantics
   * as removeEdge(dst, src).
   */
  void RemoveEdge(Node* src, Node* dst);

  /**
   * Prints the graph in DOT file format to the output stream.
   */
  void ToString(std::ostream& out);

  /**
   * Writes the graph to the specified filename in the DOT format.
   *
   * Returns true if successful, false otherwise.
   */
  bool WriteToFile(std::string& filename);

};


}  // namespace DotWriter

#endif /* GRAPH_H_ */
