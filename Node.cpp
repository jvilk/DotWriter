#include "Node.h"
#include "Util.h"

#include <sstream>

namespace DotWriter {

/**
 * Prints out a string representation of the node (essentially, a line describing it for the DOT file).
 *
 * Don't make fun of me for emulating a Java thing...
 */
const std::string Node::toString() {
    std::stringstream ss;

    //Node identifier in the DOT file.
    ss << "\tn" << getId();

    //Label
    if (getLabel() != "") {
        ss << " [label=\"" << sanitizeLabel(getLabel()) << "\"]";
    }

    //Line ending semicolon and newline.
    ss << ";\n";

    return ss.str();
}

};  // namespace DotWriter