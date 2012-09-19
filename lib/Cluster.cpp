#include "Cluster.h"

namespace DotWriter {

void Cluster::Print(std::ostream& out, unsigned tabDepth) {
  std::string linePrefix = std::string(tabDepth*_tabIncrement, _tabCharacter);
  std::string titlePrefix =
    std::string((tabDepth-1)*_tabIncrement, _tabCharacter);

  out << titlePrefix << "subgraph " << GetId() << " {\n";

  if (_label.compare("") != 0) {
    _attributes.AddCustomAttribute("label", _label);
  }

  if (!_attributes.Empty()) {
    _attributes.Print(out, linePrefix);
    out << ";\n";
  }

  PrintNECS(out, tabDepth);

  out << titlePrefix << "}\n";
}

}  // namespace DotWriter
