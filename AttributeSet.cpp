#include "AttributeSet.h"
#include "Node.h"

namespace DotWriter {

void GraphAttributeSet::SetRoot(Node* node) {
  //TODO(jvilk): If node is deleted, this is not cleaned up...
  AddSimpleAttribute<std::string>(AttributeType::ROOT, node->GetId());
}

}  // namespace DotWriter