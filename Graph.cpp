/*
 * Implementation of the graph class and related classes.
 *
 * Author: John Vilk (jvilk@cs.umass.edu)
 */
#include "Graph.h"
#include "Subgraph.h"
#include "Cluster.h"

namespace DotWriter {

Graph::~Graph() {
  std::vector<Node *>::iterator nodeIt;
  for (nodeIt = _nodes.begin(); nodeIt != _nodes.end(); nodeIt++) {
    delete *nodeIt;
  }

  std::vector<Edge *>::iterator edgeIt;
  for (edgeIt = _edges.begin(); edgeIt != _edges.end(); edgeIt++) {
    delete *edgeIt;
  }

  std::vector<Subgraph *>::iterator sgIt;
  for (sgIt = _subgraphs.begin(); sgIt != _subgraphs.end(); sgIt++) {
    delete *sgIt;
  }
}

Subgraph* Graph::AddSubgraph(bool isDigraph, std::string label) {
  Subgraph* sg = new Subgraph(_idManager->GetSubgraphId(), _idManager,
    isDigraph, label);
  _subgraphs.push_back(sg);
  return sg;
}

Subgraph* Graph::AddSubgraph(bool isDigraph, const std::string& label,
  const std::string& id) {
  std::string sanitizedId = _idManager->ValidateCustomId(id);
  Subgraph* sg = new Subgraph(sanitizedId, _idManager, isDigraph, label);
  _subgraphs.push_back(sg);
  return sg;
}

void Graph::RemoveSubgraph(Subgraph* subgraph) {
  std::vector<Subgraph*>::iterator it = std::find(_subgraphs.begin(),
    _subgraphs.end(), subgraph);

  if (it != _subgraphs.end())
    _subgraphs.erase(it);

  delete subgraph;
}

Cluster* Graph::AddCluster(bool isDigraph, std::string label) {
  Cluster* cluster = new Cluster(_idManager->GetSubgraphId(), _idManager,
    isDigraph, label);
  _clusters.push_back(cluster);
  return cluster;
}

Cluster* Graph::AddCluster(bool isDigraph, const std::string& label,
  const std::string& id) {
  std::string sanitizedId = _idManager->ValidateCustomId(id);
  Cluster* cluster = new Cluster(sanitizedId, _idManager, isDigraph, label);
  _clusters.push_back(cluster);
  return cluster;
}

void Graph::RemoveCluster(Cluster* cluster) {
  std::vector<Cluster*>::iterator it = std::find(_clusters.begin(),
    _clusters.end(), cluster);

  if (it != _clusters.end())
    _clusters.erase(it);

  delete cluster;
}

void Graph::PrintNECS(std::ostream& out) {
  //Default styles.
  if (!_defaultNodeAttributes.Empty()) {
    out << "node [";
    _defaultNodeAttributes.Print(out);
    out << "];";
  }

  if (!_defaultEdgeAttributes.Empty()) {
    out << "edge [";
    _defaultEdgeAttributes.Print(out);
    out << "];";
  }

  //Output nodes
  std::vector<Node*>::iterator nodeIt;
  for (nodeIt = _nodes.begin(); nodeIt != _nodes.end(); nodeIt++) {
    Node* node = *nodeIt;
    node->Print(out);
  }

  //Output edges
  std::vector<Edge*>::iterator edgeIt;
  for (edgeIt = _edges.begin(); edgeIt != _edges.end(); edgeIt++) {
    Edge* edge = *edgeIt;
    edge->Print(IsDigraph(), out);
  }

  //Output subgraphs.
  std::vector<Subgraph*>::iterator sgIt;
  for (sgIt = _subgraphs.begin(); sgIt != _subgraphs.end(); sgIt++) {
    Subgraph* sg = *sgIt;
    sg->Print(out);
  }

  //Output cluster subgraphs.
  std::vector<Cluster*>::iterator cIt;
  for (cIt = _clusters.begin(); cIt != _clusters.end(); cIt++) {
    Cluster* cluster = *cIt;
    cluster->Print(out);
  }
}

}  // namespace DotWriter