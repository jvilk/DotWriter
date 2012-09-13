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
#include "AttributeSet.h"

namespace DotWriter {

class Subgraph;
class Cluster;

/**
 * Represents a graph. It's a bag of nodes and edges, basically.
 */
class Graph {
protected:
  bool _isDigraph;
  IdManager* _idManager;   // Managed by root graph.
  const std::string& _id;  // Reference to unique ID stored in the _idManager.
                           // Not really all that important, as you don't
                           // reference this in the DOT file. Dotty seems to
                           // want one on the master graph regardless.
  // I use vector since output order matters.
  std::string _label;
  std::vector<Node *> _nodes;
  std::vector<Edge *> _edges;
  std::vector<Subgraph *> _subgraphs;
  std::vector<Cluster *> _clusters;
  NodeAttributeSet _defaultNodeAttributes;
  EdgeAttributeSet _defaultEdgeAttributes;

public:
  /**
   * Constructs a new Graph object.
   * - isDigraph: Set to 'true' if this is a directed graph.
   * - label: Text that is printed somewhere adjacent to the graph.
   * - id: Custom id (optional)
   */
  Graph(IdManager* idManager, bool isDigraph = false, std::string label = "",
    std::string id = "Graph") :
    _isDigraph(isDigraph), _idManager(idManager),
    _id(_idManager->ValidateCustomId(id)), _label(label),
    _defaultNodeAttributes(NodeAttributeSet()),
    _defaultEdgeAttributes(EdgeAttributeSet()) {
  }

  virtual ~Graph();

  /** Simple getters and setters **/

  bool IsDigraph() {
    return _isDigraph;
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
  Subgraph* AddSubgraph(bool isDigraph = false, std::string label = "");
  Subgraph* AddSubgraph(bool isDigraph, const std::string& label,
    const std::string& id);

  /**
   * Remove the given subgraph from this graph.
   */
  void RemoveSubgraph(Subgraph* subgraph);

  /**
   * Create a new cluster on this graph.
   */
  Cluster* AddCluster(bool isDigraph = false, std::string label = "");
  Cluster* AddCluster(bool isDigraph, const std::string& label,
    const std::string& id);

  /**
   * Remove the given cluster from this graph.
   */
  void RemoveCluster(Cluster* cluster);

  /**
   * Constructs a Node object, adds it to the graph, and returns it.
   */
  Node* AddNode() {
    Node* node = new Node(_idManager->GetNodeId());
    _nodes.push_back(node);
    return node;
  }

  /**
   * Constructs a Node object with the given label, adds it to the graph, and
   * returns it.
   */
  Node* AddNode(const std::string& label) {
    Node* node = new Node(_idManager->GetNodeId(), label);
    _nodes.push_back(node);
    return node;
  }

  Node* AddNode(const std::string& label, const std::string& id) {
    Node* node = new Node(_idManager->ValidateCustomId(id), label);
    _nodes.push_back(node);
    return node;
  }

  /**
   * Removes the node from the graph.
   *
   * Note that this function deletes the node object, and removes all edges
   * to/from it. Also, note that it is currently more expensive than you may
   * expect -- O(|E|).
   * TODO(jvilk): Actually delete those edges...
   */
  void RemoveNode(Node* node) {
    std::vector<Node*>::iterator it = std::find(_nodes.begin(), _nodes.end(),
      node);

    if (it != _nodes.end()) {
      _nodes.erase(it);
    }

    delete node;
  }

  /**
   * Add an edge to the graph. Returns a reference to the edge that can be
   * manipulated to change edge properties.
   */
  Edge* AddEdge(Node* src, Node* dst) {
    Edge* edge = new Edge(src, dst);
    _edges.push_back(edge);
    return edge;
  }
  Edge* AddEdge(Node* src, Node* dst, const std::string& label) {
    Edge* edge = new Edge(src, dst, label);
    _edges.push_back(edge);
    return edge;
  }

  /**
   * Removes the edge from the graph. Note that this also deletes the GEdge
   * object.
   */
  void RemoveEdge(Edge* edge) {
    std::vector<Edge*>::iterator it = std::find(_edges.begin(), _edges.end(),
      edge);

    if (it != _edges.end())
      _edges.erase(it);

    delete edge;
  }

  /**
   * Removes any edges from src to dst from the graph. Note that this version
   * of the function is currently O(|E|).
   *
   * If this is not a digraph, then removeEdge(src, dst) has the same semantics
   * as removeEdge(dst, src).
   * TODO(jvilk): Implement.
   */
  //void RemoveEdge(Node* src, Node* dst);

  virtual void Print(std::ostream& out) = 0;

protected:
  /**
   * Prints nodes, edges, cluster subgraphs, and subgraphs.
   */
  void PrintNECS(std::ostream& out);
};


}  // namespace DotWriter

#endif /* GRAPH_H_ */
