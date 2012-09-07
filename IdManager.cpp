#include "IdManager.h"

namespace DotWriter {

const std::string& IdManager::GetNodeId() {
  std::stringstream nodeStream;
  nodeStream << "Node" << GetNextNodeIdNum();

  const std::string idStr = nodeStream.str();
  std::pair<const std::string&, bool> retVal = RegisterId(idStr);

  if (retVal.second) {
    return retVal.first;
  }

  // Recurse if the chosen graph ID was already taken. This means that
  // the user must have manually specified an ID with the name "Node##".
  return GetNodeId();
}

const std::string& IdManager::GetSubgraphId() {
  std::stringstream graphStream;
  graphStream << "Graph" << GetNextSubgraphIdNum();

  const std::string idStr = graphStream.str();
  std::pair<const std::string&, bool> retVal = RegisterId(idStr);

  if (retVal.second) {
    return retVal.first;
  }

  // Recurse if the chosen graph ID was already taken. This means that
  // the user must have manually specified an ID with the name "Graph##".
  return GetSubgraphId();
}

const std::string& IdManager::ValidateCustomId(std::string& customId) {
  std::pair<const std::string&, bool> retVal = RegisterId(customId);
  if (retVal.second) {
    return retVal.first;
  }

  std::ostringstream oss;
  std::string newCustomId;

  while (!retVal.second) {
    oss << customId << GetNextCustomIdNum(); 

    newCustomId = oss.str();
    retVal = RegisterId(newCustomId);
    oss.clear();
  }

  return retVal.first;
}

}  // namespace DotWriter