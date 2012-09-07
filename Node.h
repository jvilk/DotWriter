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
  const std::string _id;
  std::string _label;
  NodeStyle::e _style;

protected:
  // Nodes should only be created / destroyed via Graph.
  // Hence, these are protected.
  Node(std::string label = "") :
    _label(label) {

  }
  
  virtual ~Node();

public:
  const std::string toString();

  /** Simple getters / setters **/
  void setStyle(NodeStyle::e style) {
    _style = style;
  }

  std::string& getLabel() {
    return _label;
  }

  void setLabel(const std::string& label) {
    _label = label;
  };

  const std::string& getId() {
    return _id;
  }
};

}  // namespace DotWriter

#endif