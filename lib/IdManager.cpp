#include "IdManager.h"

#include <sstream>

namespace DotWriter {

const std::string& IdManager::GetNodeId() {
  std::stringstream nodeStream;
  nodeStream << "Node" << GetNextNodeIdNum();

  const std::string idStr = nodeStream.str();
  bool success;
  const std::string& val = RegisterId(&success, idStr);

  if (success) {
    return val;
  }

  // Recurse if the chosen graph ID was already taken. This means that
  // the user must have manually specified an ID with the name "Node##".
  return GetNodeId();
}

const std::string& IdManager::GetSubgraphId() {
  std::stringstream graphStream;
  graphStream << "Graph" << GetNextSubgraphIdNum();

  const std::string idStr = graphStream.str();
  bool success;
  const std::string& val = RegisterId(&success, idStr);

  if (success) {
    return val;
  }

  // Recurse if the chosen graph ID was already taken. This means that
  // the user must have manually specified an ID with the name "Graph##".
  return GetSubgraphId();
}

const std::string& IdManager::GetClusterId() {
  std::stringstream graphStream;
  graphStream << "cluster_" << GetNextSubgraphIdNum();

  const std::string idStr = graphStream.str();
  bool success;
  const std::string& val = RegisterId(&success, idStr);

  if (success) {
    return val;
  }

  // Recurse if the chosen graph ID was already taken. This means that
  // the user must have manually specified an ID with the name "Graph##".
  return GetClusterId();
}

const std::string& IdManager::ValidateCustomId(std::string customId) {
  bool success;
  const std::string& val = RegisterId(&success, customId);

  if (success) {
    return val;
  }

  std::ostringstream oss;
  std::string newCustomId;

  while (!success) {
    oss << customId << GetNextCustomIdNum(); 

    newCustomId = oss.str();
    const std::string& val2 = RegisterId(&success, newCustomId);
    if (success) return val2;
    oss.clear();
  }

  // Unreachable.
  return val;
}

const std::string& IdManager::ValidateCustomClusterId(std::string customId) {
  // Ensure it begins with 'cluster'
  if (customId.substr(0,7).compare("cluster") != 0) {
    customId = "cluster" + customId;
  }

  return ValidateCustomId(customId);
}

}  // namespace DotWriter
