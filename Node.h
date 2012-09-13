#ifndef DOTWRITER_NODE_H_
#define DOTWRITER_NODE_H_

#include <ostream>
#include <string>

#include "AttributeSet.h"

namespace DotWriter {

/**
 * Represents a node in a graph.
 */
class Node {
private:
  const std::string& _id; // Reference to string stored in Graph's _idManager.
  std::string _label;
  NodeAttributeSet _attributes;

public:
  Node(const std::string& id, std::string label = "") :
    _id(id), _label(label) {
  }
  virtual ~Node() {};

  void Print(std::ostream& out);

  /** Simple getters / setters **/
  const std::string& GetLabel() {
    return _label;
  }

  void SetLabel(std::string label) {
    _label = label;
  };

  const std::string& GetId() {
    return _id;
  }

  NodeAttributeSet& GetAttributes() {
    return _attributes;
  }
};

}  // namespace DotWriter

#endif
