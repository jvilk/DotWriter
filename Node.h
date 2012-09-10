#ifndef DOTWRITER_NODE_H_
#define DOTWRITER_NODE_H_

#include "Enums.h"

#include <string>

namespace DotWriter {

/**
 * Represents a node in a graph.
 */
class Node {
private:
  const std::string& _id; // Reference to string stored in Graph's _idManager.
  std::string _label;
  NodeStyle::e _style;

protected:
  // Nodes should only be created / destroyed via Graph.
  // Hence, these are protected.
  Node(const std::string& id, std::string label = "") :
    _id(id), _label(label) {

  }
  
  virtual ~Node();

public:
  std::string ToString();

  /** Simple getters / setters **/
  void SetStyle(NodeStyle::e style) {
    _style = style;
  }

  const std::string& GetLabel() {
    return _label;
  }

  void SetLabel(std::string label) {
    _label = label;
  };

  const std::string& GetId() {
    return _id;
  }
};

}  // namespace DotWriter

#endif