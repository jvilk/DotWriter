#include "Node.h"
#include "Util.h"

namespace DotWriter {

/**
 * Prints out a string representation of the node (essentially, a line
 * describing it for the DOT file).
 */
void Node::Print(std::ostream& out) {
    //Node identifier in the DOT file.
    out << GetId();

    //Label
    if (GetLabel() != "") {
        _attributes.AddCustomAttribute("label", GetLabel());
    }

    if (!_attributes.Empty()) {
      out << " [";
      _attributes.Print(out);
      out << "]";
    }

    //Line ending semicolon and newline.
    out << ";\n";
}

};  // namespace DotWriter