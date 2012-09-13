#include "Node.h"
#include "Util.h"

namespace DotWriter {

/**
 * Prints out a string representation of the node (essentially, a line describing it for the DOT file).
 *
 * Don't make fun of me for emulating a Java thing...
 */
void Node::ToString(std::ostream& out) {
    //Node identifier in the DOT file.
    out << GetId();

    //Label
    if (GetLabel() != "") {
        _attributes.AddCustomAttribute("label", GetLabel());
    }

    out << "[";
    _attributes.ToString(out);
    out << "]";

    //Line ending semicolon and newline.
    out << ";\n";
}

};  // namespace DotWriter