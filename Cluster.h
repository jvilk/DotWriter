#ifndef DOTWRITER_CLUSTER_H_
#define DOTWRITER_CLUSTER_H_

#include "AttributeSet.h"
#include "Graph.h"

namespace DotWriter {

class Cluster : public Graph {
private:
  ClusterAttributeSet _attributes;

public:
  Cluster(const std::string& id, IdManager* idManager, bool isDigraph = false,
    std::string label = "") : Graph(idManager, isDigraph, label, id),
    _attributes(ClusterAttributeSet()) {
  }

  virtual ~Cluster() {};

  /** Getters and setters **/
  ClusterAttributeSet& GetAttributes() {
    return _attributes;
  }

  virtual void Print(std::ostream& out);
};

}  // namespace DotWriter

#endif